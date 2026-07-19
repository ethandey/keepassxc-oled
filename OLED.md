# KeePassXC OLED fork

Fork of [KeePassXC](https://github.com/keepassxreboot/keepassxc) **2.7.12** with a pure-black OLED dark theme.

## What changed

| Area | Change |
|------|--------|
| Dark palette | True `#000000` window chrome; near-black (`#0A0A0A` / `#121212`) content surfaces |
| Default theme | `dark` (was `auto`) |
| App name | `KeePassXC-OLED` / display **KeePassXC OLED** |
| Bundle ID | `org.keepassxc.keepassxc-oled` |
| Config dir | Separate from stock (`~/Library/Application Support/KeePassXC-OLED/`) |

Stock `/Applications/KeePassXC.app` is **not** replaced. Install side-by-side as `KeePassXC-OLED.app`.

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
