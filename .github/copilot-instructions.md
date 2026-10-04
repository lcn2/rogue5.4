# Copilot instructions for rogue5.4

See `.github/CODE_REVIEW.md` for the full review guide.

- Pull requests target the `rogue54-audit` branch, never `master`.
- C is GNU17, must build warning-free with current `clang` and `gcc` (`make clang`, `make gcc`).
- `make clang-format` must leave the sources unchanged.
- Stay portable to current macOS, Linux (RHEL10) and NetBSD.
- Do not edit generated files: `rogue.6`, `rogue.me`, `rogue.html`, `rogue.doc`,
  `rogue.cat`, `rogue.md`; edit the matching `*.in` file and run `make stddocs`.
- `version[]` in `vers.c` stays exactly `5.4.5`; only update `release[]` (YYYY-MM-DD) when code changes.
- The wizard string `bathtub` is intentional, public, historic game behavior; it is not a secret.
- Wizard mode permanently suppresses score recording once enabled, even if later disabled.
- `$SEED` is honored only in wizard mode; `$ROGOSEED` only when the player name starts with `rogo-`
  (and that `rogo-` name is kept when such a seed is used).
- Never document or support running rogue setuid/setgid (see `SECURITY.md`).
