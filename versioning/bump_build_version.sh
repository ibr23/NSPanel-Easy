#!/bin/bash

BUILD_VERSION_FILE="./versioning/BUILD_VERSION"
BUILD_VERSION_YAML_FILE="./versioning/BUILD_VERSION_YAML"

# Read the current build version
if [ -f "$BUILD_VERSION_FILE" ]; then
  CURRENT_BUILD_VERSION=$(cat "$BUILD_VERSION_FILE")
else
  CURRENT_BUILD_VERSION="0.0.0" # Default if file doesn't exist
fi

# Validate the current version format
if ! [[ "$CURRENT_BUILD_VERSION" =~ ^[0-9]{4}\.[0-9]{1,2}\.[0-9]+$ ]]; then
  echo "Error: Invalid build version format in $BUILD_VERSION_FILE"
  exit 1
fi

# Extract components
CURRENT_YEAR=$(date +%Y)
CURRENT_MONTH=$(date +%-m) # Avoid leading zero for the month
CURRENT_SEQ=$(echo "$CURRENT_BUILD_VERSION" | awk -F. '{print $3}')

BUILD_VERSION_YEAR=$(echo "$CURRENT_BUILD_VERSION" | awk -F. '{print $1}')
BUILD_VERSION_MONTH=$(echo "$CURRENT_BUILD_VERSION" | awk -F. '{print $2}')

# Determine new build version
if [[ "$CURRENT_YEAR" == "$BUILD_VERSION_YEAR" && "$CURRENT_MONTH" == "$BUILD_VERSION_MONTH" ]]; then
  NEXT_SEQ=$((CURRENT_SEQ + 1))
else
  NEXT_SEQ=1 # Reset sequence for a new month
fi

NEW_BUILD_VERSION="${CURRENT_YEAR}.${CURRENT_MONTH}.${NEXT_SEQ}"

# Update the plain text BUILD_VERSION file
echo "$NEW_BUILD_VERSION" > "$BUILD_VERSION_FILE"

# Update the YAML BUILD_VERSION_YAML file
echo "BUILD_VERSION: $NEW_BUILD_VERSION" > "$BUILD_VERSION_YAML_FILE"

# Commit and tag
if ! git add "$BUILD_VERSION_FILE" "$BUILD_VERSION_YAML_FILE"; then
  echo "Error: Failed to stage build version files"
  exit 1
fi
if ! git commit -m "Bump build version to $NEW_BUILD_VERSION [skip-versioning]"; then
  echo "Error: Failed to commit build version bump"
  exit 1
fi
