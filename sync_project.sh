#!/bin/bash

# Set your repo URLs and local directories


REPO1_DIR="/Users/charlesgoncalves/git/dei/so-dei"
REPO2_DIR="/Users/charlesgoncalves/git/dei/so"
PROJECT_SUBDIR="project"


# Pull latest changes
cd "$REPO1_DIR" && git pull && cd ..
cd "$REPO2_DIR" && git pull && cd ..

# Rsync from repo1 to repo2 (you can reverse this direction if needed)
rsync -av --delete "$REPO1_DIR/$PROJECT_SUBDIR/" "$REPO2_DIR/$PROJECT_SUBDIR/"

# Check if there are changes and commit them
cd "$REPO2_DIR"
if [ -n "$(git status --porcelain)" ]; then
    git add "$PROJECT_SUBDIR"
    git commit -m "Sync project folder from repo1"
    git push
else
    echo "No changes to sync."
fi
cd ..

echo "✅ Sync complete!"
