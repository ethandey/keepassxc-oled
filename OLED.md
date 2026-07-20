# KeePassXC OLED theme (contribution-shaped)

Based on [KeePassXC](https://github.com/keepassxreboot/keepassxc) **2.7.12**.

## User-facing behaviour (upstream-friendly)

| Control | Effect |
|---------|--------|
| **View → Theme → Dark (OLED)** | Pure-black UI; live apply (no restart) |
| **View → Theme → Dark** | Stock KeePassXC gray dark (unchanged) |
| **View → Theme → Light / Auto / Classic** | Unchanged (Classic still restarts) |
| macOS title bar | Pure black **only** while Dark (OLED) is selected |

Config key: `GUI/ApplicationTheme=oled`

## Local packaging (not for upstream PR)

| Area | Local fork only |
|------|-----------------|
| App name | `KeePassXC-OLED` / bundle `org.keepassxc.keepassxc-oled` |
| Updates | Compiled with `WITH_XC_UPDATECHECK=OFF` |
| Deploy | `scripts/deploy-macos-oled.sh` → `/Applications/KeePassXC-OLED.app` |

## Build (macOS arm64)

```bash
export PATH="$HOME/.homebrew/bin:$PATH"
brew install qt@5 argon2 botan qrencode readline minizip zlib libsodium cmake ninja

cd /path/to/keepassxc-oled
cmake -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_PREFIX_PATH="$(brew --prefix qt@5)" \
  -DWITH_XC_ALL=ON \
  -DWITH_TESTS=OFF \
  -DWITH_XC_DOCS=OFF \
  -DKEEPASSXC_BUILD_TYPE=Release

cmake --build build -j"$(sysctl -n hw.ncpu)"
```

App bundle: `build/src/KeePassXC-OLED.app`

## Install (side-by-side, no system overwrite)

```bash
# Optional: ad-hoc sign for local Gatekeeper
codesign --force --deep --sign - build/src/KeePassXC-OLED.app

rm -rf /Applications/KeePassXC-OLED.app
cp -R build/src/KeePassXC-OLED.app /Applications/
xattr -dr com.apple.quarantine /Applications/KeePassXC-OLED.app 2>/dev/null || true
```

## Touch ID (Quick Unlock)

1. Open **KeePassXC OLED** (not stock).
2. Unlock your database with the **master password** once.
3. **Tools → Settings → Security → Convenience**  
   Enable **Enable database quick unlock (Touch ID / Windows Hello)**.
4. Allow Keychain access if macOS prompts.
5. **Database → Lock** (⌘L), then Unlock → Touch ID.

First unlock after every app quit still requires the master password; subsequent unlocks while the app is running use Touch ID.

## Safety notes

- Does not modify stock KeePassXC, SIP, or system frameworks.
- Database format is standard `.kdbx` (compatible with stock KeePassXC).
- Keep the master password; biometrics are a convenience unlock only.

## Deploy after rebuild

```bash
cmake --build build -j"$(sysctl -n hw.ncpu)"
./scripts/deploy-macos-oled.sh
```

This rewrites plugin dylibs to use the app’s Frameworks (required so Auto-Type
does not load a second copy of Qt and crash).
