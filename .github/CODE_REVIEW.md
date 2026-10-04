# Code review guide

## Checklist
1. Memory safety: bounds on every buffer write (use `strlcpy`/`strlcat`/`snprintf`), no leaks on
   early return, no use-after-free of list/object nodes, no unchecked `malloc`/`fopen` results.
2. Save/restore (`save.c`, `state.c`): validate sizes and indices read from files; never trust them.
3. Environment and file handling: `$HOME`, `$ROGUEOPTS`, `$SEED`, `$ROGOSEED`, lock/save/score paths.
4. Wizard mode: score recording stays permanently disabled once wizard mode was ever enabled.
5. Portability: macOS, Linux/RHEL10, NetBSD; no GNU-only or BSD-only calls without a fallback.
6. Docs match code: edit `rogue.6.in`, `rogue.me.in`, `rogue.html.in`, `rogue.md.in`, `rogue.cat.in`
   (never generated outputs), run `make stddocs`, keep `README.md` tables complete and correct.
7. Typos in C comments and docs.
8. Releases: `version[]` stays `5.4.5`; bump only `release[]` (YYYY-MM-DD) when code changes.

## Required checks
    make clobber && make clang
    make clobber && make gcc
    make clobber && make asan      # run a short game under ASAN
    make clang-format && git diff --exit-code
    make stddocs

## Out of scope
Setuid/setgid operation is unsupported (see `SECURITY.md`).
