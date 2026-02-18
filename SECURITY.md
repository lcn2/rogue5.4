# Reporting Security Issues

**IMPORTANT NOTE**: We do **NOT** support running `rogue(6)` with
setuid and/or setgid privileges.  Please do **NOT** file security reports
related to running `rogue(6)` setuid and/or setgid!

**IMPORTANT NOTE**: While a number of significant bugs have been fixed,
we are sure that there remain other bugs.  As such, we do **NOT**
recommend, nor support running rogue setuid/setgid.  We recommend that
you maintain the `Makefile` default of **NOT** setting the `GROUPOWNER`
make variable (leave it empty).  While you may need to `sudo(1)` in
order to install rogue under `/usr/local/bin/`, `/usr/local/share/`,
and `/usr/local/man/man6/`, you do **NOT** have to run `rogue(6)` with
any privileges!  Just run `rogue(6)` as yourself and let the game use
"dot-files" under your home directory.

With the above in mind, we take security bugs seriously.  We appreciate
your efforts to responsibly disclose your findings, and will make every
effort to acknowledge your contributions for any verified security issues
when they have been fixed.

To report a security issue, click on: "[Open a draft security advisory](https://github.com/lcn2/rogue5.4/security/advisories/new)"

We will send a response indicating the next steps in handling your
report. After the initial reply to your report, we will keep you informed
of the progress towards a fix and full announcement, and may ask for
additional information or guidance.


## Supported Versions

rogue version 5.4.5 release 2026-02-18 (chongo was here), or later.
