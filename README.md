# KeePassXC OLED (mac)

Custom **KeePassXC 2.7.12** build with pure-black OLED theme + Touch ID (Quick Unlock).  
Installed as **`/Applications/KeePassXC-OLED.app`** only. Stock KeePassXC is removed.

**Source of truth:** this directory (`side-projects/devices/mac/keepass`)  
**GitHub fork:** https://github.com/ethandey/keepassxc-oled (`oled-theme` branch)

---

## Is this “just a theme dropdown”?

**Yes for the feature** (how you’d contribute it). **Local packaging is extra.**

| Setting | What it does |
|---------|----------------|
| View → Theme → **Dark** | Stock gray dark (**unchanged**) |
| View → Theme → **Dark (OLED)** | Pure-black UI + black macOS title bar; **applies live** (no restart) |
| Light / Auto / Classic | Unchanged (Classic still asks to restart) |

Config: `GUI/ApplicationTheme=oled`

**Upstream PR surface:** `OledStyle` + QSS + menu item + `applyTheme("oled")` + optional macOS title-bar hook.  
**Not for PR:** app rename, update-check off, deploy script (those are local only).

---

## How invasive is this to the system?

| Touch | Risk | Notes |
|-------|------|--------|
| `/Applications/KeePassXC-OLED.app` | Low | Your custom app only |
| Removed `/Applications/KeePassXC.app` | Low | Stock uninstalled |
| `~/Library/Application Support/KeePassXC OLED/` | Low | Config for OLED only |
| Homebrew build deps (`~/.homebrew`: qt@5, botan, …) | Low | User-space; not required at runtime if app is deployed |
| SIP / system frameworks / other apps | **None** | Untouched |
| Database `.kdbx` format | **None** | Standard; works with other KeePassXC clients |

**Not** a system theme, login item, kernel extension, or patched system KeePass.

---

## Installed layout

```
/Applications/KeePassXC-OLED.app     ← use this
# stock KeePassXC.app                ← removed

~/Library/Application Support/KeePassXC OLED/keepassxc.ini
~/Library/Caches/KeePassXC OLED/     (local state / last DB path)
```

Stock’s old config may still live under `~/Library/Application Support/KeePassXC/` — harmless leftover prefs.

---

## Updates

- **OLED build:** update checks **disabled at compile time**.  
- It will **not** auto-download official KeePassXC upgrades (those would wipe OLED styling).  
- To “update”: rebase/merge upstream tags in this fork, rebuild, redeploy.

---

## Touch ID (Quick Unlock)

1. Open **KeePassXC OLED**  
2. Unlock once with master password  
3. **Tools → Settings → Security → Convenience**  
   → Enable **database quick unlock (Touch ID / …)**  
4. Lock (⌘L) → unlock with Touch ID  

First unlock after quit still needs the password.

---

## Rebuild / redeploy

```bash
export PATH="$HOME/.homebrew/bin:$PATH"
cd ~/side-projects/devices/mac/keepass

cmake -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_PREFIX_PATH="$HOME/.homebrew/opt/qt@5;$HOME/.homebrew" \
  -DWITH_XC_ALL=ON \
  -DWITH_TESTS=OFF \
  -DWITH_XC_DOCS=OFF \
  -DWITH_XC_UPDATECHECK=OFF \
  -DKEEPASSXC_BUILD_TYPE=Release

cmake --build build -j"$(sysctl -n hw.ncpu)"
./scripts/deploy-macos-oled.sh
```

`deploy-macos-oled.sh` runs `macdeployqt`, rewrites plugin dylib paths, ad-hoc signs, and installs to `/Applications/KeePassXC-OLED.app`.

More detail: [OLED.md](./OLED.md)

---

## Contribution-shaped code (theme)

| File | Role |
|------|------|
| `src/gui/styles/oled/*` | OledStyle palette + QSS |
| `src/gui/MainWindow.ui` / `.cpp` | View → Theme → Dark (OLED) |
| `src/gui/Application.cpp` | `applyTheme()` branch for `oled` + macOS chrome toggle |
| `src/gui/osutils/macutils/AppKitImpl.mm` | Title bar black only when OLED active |

## Local-only packaging

| File | Role |
|------|------|
| `src/main.cpp` / Info.plist / CMake `PROGNAME` | KeePassXC-OLED app identity |
| `scripts/deploy-macos-oled.sh` | Bundle + install |

---

## Safety / rollback

- **Use stock again:** install official DMG/cask; your `.kdbx` still works.  
- **Remove OLED only:** `rm -rf /Applications/KeePassXC-OLED.app`  
- **This repo:** full source to rebuild anytime.
