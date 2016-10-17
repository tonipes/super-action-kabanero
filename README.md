# Kabanero

> Powered by sausage 🌭 and the hot stuff 🌶

[![build status](https://gitlab.com/mureke/kabanero/badges/master/build.svg)](https://gitlab.com/mureke/kabanero/commits/master)
[![coverage report](https://gitlab.com/mureke/kabanero/badges/master/coverage.svg)](https://gitlab.com/mureke/kabanero/commits/master)

## Dependencies

Install [SFML](http://www.sfml-dev.org/) with

```
brew install sfml
```

or your environment's equivalent.

## Tools

To build the project, you can use `build.sh` script.

*   To make use argument: `./build.sh make`
*   To run after building: `./build.sh run`
*   To clean up: `./build.sh clean`

## Development environment

Please use [atom](https://atom.io/).

Some useful packages for making development more enjoyable:

*   [autocomplete-clang](https://atom.io/packages/autocomplete-clang)
*   [autocomplete-emojis](https://atom.io/packages/autocomplete-emojis)
*   [build](https://atom.io/packages/build)
*   [linter-clang](https://atom.io/packages/linter-clang)


## Contributing

### Git Hooks

Activate git hooks by running command `activate_hooks.sh` before committing.

### Commit message

Always start commit messages with one of these emojis:

**Note** that you don't put the actual emoji in the message.
Please use the markdown emoji format eg. `:tada:` instead of 🎉

Commit Type             | Emoji
----------------------- | -------------------
Initial Commit          | 🎉  `:tada:`
Version Tag             | 🔖  `:bookmark:`
New Feature             | ✨  `:sparkles:`
Bugfix                  | 🐛  `:bug:`
Refactoring             | 📦  `:package:`
Documentation           | 📚  `:books:`
Performance             | 🐎  `:racehorse:`
Cosmetic                | 💄  `:lipstick:`
Tooling                 | 🔧  `:wrench:`
Tests                   | 🚨  `:rotating_light:`
Deprecation             | 💩  `:poop:`
Work In Progress (WIP)  | 🚧  `:construction:`
Library                 | 📓  `:notebook:`
Remove linter warnings  | 👕  `:shirt:`
Remove code/files       | 🔥  `:fire:`
