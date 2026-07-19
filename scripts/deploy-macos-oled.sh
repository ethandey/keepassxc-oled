#!/usr/bin/env bash
# Deploy KeePassXC-OLED.app for local use (side-by-side with stock KeePassXC).
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
export PATH="${HOME}/.homebrew/bin:${HOME}/.homebrew/opt/qt@5/bin:${PATH:-}"

APP_BUILD="${ROOT}/build/src/KeePassXC-OLED.app"
DEST="/Applications/KeePassXC-OLED.app"

if [[ ! -d "$APP_BUILD" ]]; then
  echo "Missing $APP_BUILD — build first (see OLED.md)" >&2
  exit 1
fi

echo "==> macdeployqt"
macdeployqt "$APP_BUILD" 2>/dev/null || true

echo "==> rewrite Homebrew install names → @executable_path"
export APP_BUILD
python3 - <<'PY'
import glob, os, re, subprocess

app = os.environ["APP_BUILD"]
fw_dir = f"{app}/Contents/Frameworks"
targets = []
targets += glob.glob(f"{app}/Contents/MacOS/*")
targets += glob.glob(f"{app}/Contents/PlugIns/**/*", recursive=True)
targets = [t for t in targets if os.path.isfile(t)]

def rewrite(path: str) -> int:
    try:
        out = subprocess.check_output(["otool", "-L", path], text=True, stderr=subprocess.DEVNULL)
    except subprocess.CalledProcessError:
        return 0
    n = 0
    for line in out.strip().splitlines()[1:]:
        dep = line.strip().split()[0]
        if not (dep.startswith("/Users/") or dep.startswith("/opt/homebrew/")):
            continue
        if ".framework/" in dep:
            m = re.search(r"(Qt[\w]+\.framework/Versions/[^/]+/[^/]+)$", dep)
            if not m:
                continue
            new = f"@executable_path/../Frameworks/{m.group(1)}"
        else:
            base = os.path.basename(dep)
            if not os.path.exists(os.path.join(fw_dir, base)):
                continue
            new = f"@executable_path/../Frameworks/{base}"
        subprocess.run(
            ["install_name_tool", "-change", dep, new, path],
            check=False, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
        )
        n += 1
    return n

total = sum(rewrite(t) for t in targets)
print(f"rewrote install names (changes attempted: {total})")
PY

echo "==> ad-hoc codesign"
codesign --force --deep --sign - "$APP_BUILD"

echo "==> install to $DEST (stock KeePassXC.app untouched)"
rm -rf "$DEST"
cp -R "$APP_BUILD" "$DEST"
xattr -dr com.apple.quarantine "$DEST" 2>/dev/null || true
codesign --force --deep --sign - "$DEST"

echo "Done. Launch: open \"$DEST\""
