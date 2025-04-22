#!/bin/sh

# Run the script to link custom hooks to git

GIT_HOOKS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.git/hooks" && pwd)"
CUSTOM_HOOKS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/hooks" && pwd)"

ln -s $CUSTOM_HOOKS_DIR/post-commit $GIT_HOOKS_DIR/post-commit
chmod +x $GIT_HOOKS_DIR/post-commit
