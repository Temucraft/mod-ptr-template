# PTR Character Templates (Temucraft fork)

This is Temucraft's fork of [azerothcore/mod-ptr-template](https://github.com/azerothcore/mod-ptr-template) by
heyitsbench (AGPL-3.0, as is the fork). What the fork changes:

- **Only on the PTR.** Nothing works unless `PtrTemplate.Enable = 1`; with `PtrTemplate.RealmId` set, only on that
  realm. So the module can be built into Live's worldserver and stays off there.
- **RBAC permissions instead of security levels:** 104101 `.template enable`, 104102 `.template disable`,
  104103 see and apply disabled templates, 104150 `.template apply`, 104151 `.template list`
  (`data/sql/db-auth/ptr_template_rbac.sql`). The five `...Security` options are gone, and the module keeps no
  `command` rows.
- **Config keys are prefixed:** `PtrTemplate.DeleteItems`, `PtrTemplate.TemplateLevel` and so on. Copy the new
  `mod_ptr_template.conf.dist` over an old `.conf`.
- **Two crash fixes:** `.template apply <name>` for an offline character now says the player wasn't found, and a
  player who logs out while a template is being applied no longer crashes the server (the steps keep the guid).
- **Temucraft level-80 sets 80-83** (tier 7 melee and ranged, spell damage, tank, healer) in
  `data/sql/db-world/ptr_template_set_temucraft.sql`. They copy everything but the tier pieces from template 3.

[mod-temucraft-ptr](https://github.com/Temucraft/mod-temucraft-ptr) uses it from the PTR Quartermaster and for
copies requested in the launcher, by running `.template apply <name> <index>`.

The upstream README follows.

---

# PTR Character Templates

- AzerothCore build status as of the [latest commit](https://github.com/heyitsbench/mod-ptr-template/commit): [![core-build](https://github.com/heyitsbench/mod-ptr-template/actions/workflows/core-build.yml/badge.svg)](https://github.com/heyitsbench/mod-ptr-template/actions/workflows/core-build.yml)

This is a module for [AzerothCore](http://www.azerothcore.org/) that adds commands for creating characters from preset templates.

[Demonstration video](https://www.youtube.com/watch?v=VPczbaUCEvw) (35s)

## Important Notes

This module depends on the selection of certain template sets that have already been filled out. The sets included in this module are based on the following:

- Template characters as seen on the Classic Era, Season of Mastery, and Wrath Classic public test realms.
- Best-in-slot gear lists from Wowhead for the various phases of Classic's release.
- Premade characters used in [VMaNGOS](https://github.com/vmangos/core).

In the future, this may be expanded to include many more sets, and anyone willing is free to contribute to this module.

Check issues page for current progress on included templates.

## How to Install

For the most part, the [guide found here](https://www.azerothcore.org/wiki/installing-a-module) can be referred to. However, the SQL portion needs a bit of attention.

**This module does have certain SQL queries in automated directories.**

Queries required to use the module are automated, as well as any completed template sets. Automated template sets are enabled and available to apply by default.

Any incomplete template set queries will need to be run manually while in development. They are disabled and need to be enabled manually (by command or editing the index entry) to be available to apply.

### (Optional) Edit Module Configuration

You are also able to edit the [mod_ptr_template.conf](https://github.com/heyitsbench/mod-ptr-template/blob/master/conf/mod_ptr_template.conf.dist) to your liking. At the time of writing, you can pick and choose what parts of the template sets it should apply, whether it announces itself on login, the security level required to apply templates, and whether it can only be applied at the start level of a character.

## Usage

By default, any account can list and apply enabled templates using `.template list` and `.template apply [template index number]` respectively. Gamemaster or higher security will also have disabled templates listed with the list command. Administrators are the only ones that can apply a disabled template. Administrators and the console are also the only ones that can enable/disable templates through the `.template enable/disable [template index number]` command.

## Creating Template Sets

For information on creating your own template sets (and submitting them here for others to use), please refer to the [wiki](https://github.com/heyitsbench/mod-ptr-template/wiki).

## How to Uninstall

To uninstall the module, you must remove the module from the `modules` directory of your AC source, rerun CMake steps depending on your OS, and recompile. You will also need to run the `uninstall.sql` on your world DB to remove vestigial entries and tables.

## Credits
- [acidmanifesto](https://github.com/acidmanifesto) for providing portions of the [skip-dk-starting-area module](https://github.com/azerothcore/mod-skip-dk-starting-area) as well as an example of storing SQL results into variables.
- [AnchyDev](https://github.com/AnchyDev) for pointing out some much needed info regarding item storage and TaskScheduler implementation.
- [Nyeriah](https://github.com/Nyeriah) for identifying a couple points of confusion to me, resulting in issues.
- [ratkosrb](https://github.com/ratkosrb) for creating the VMaNGOS premade character sets and allowing them to be adapted for this module.
