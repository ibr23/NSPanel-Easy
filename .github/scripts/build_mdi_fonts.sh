#!/usr/bin/env bash
# build_mdi_fonts.sh
# ==================
# Convenience wrapper for build_mdi_fonts.py.
#
# Expected directory layout (sibling repos):
#   ../MaterialDesign-SVG/   <- Templarian/MaterialDesign-SVG clone
#   ../NSPanel-Easy/         <- this repo
#
# To clone the MDI SVG repo (first time only):
#   git clone https://github.com/Templarian/MaterialDesign-SVG.git \
#     "$(dirname "$SCRIPT_DIR")/MaterialDesign-SVG"
#
# Usage:
#   .github/scripts/build_mdi_fonts.sh [--dry-run]

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_DIR="$(cd "${SCRIPT_DIR}/../.." && pwd)"
SVG_DIR="$(cd "${REPO_DIR}/../MaterialDesign-SVG" 2>/dev/null && pwd)" || {
  echo "ERROR: MaterialDesign-SVG repo not found at ${REPO_DIR}/../MaterialDesign-SVG"
  echo "  Clone it with:"
  echo "    git clone https://github.com/Templarian/MaterialDesign-SVG.git \\"
  echo "      \"${REPO_DIR}/../MaterialDesign-SVG\""
  exit 1
}

OUTPUT_DIR="${REPO_DIR}/../mdi-build"
DOCS_DIR="${REPO_DIR}/docs/icons"
HEADER_DIR="${REPO_DIR}/components/nspanel_easy"

echo "Repo:      ${REPO_DIR}"
echo "SVG dir:   ${SVG_DIR}"
echo "Output:    ${OUTPUT_DIR}"
echo "Docs:      ${DOCS_DIR}"
echo "Header:    ${HEADER_DIR}"
echo ""

python3 "${SCRIPT_DIR}/build_mdi_fonts.py" \
  --svg-dir    "${SVG_DIR}"    \
  --output-dir "${OUTPUT_DIR}" \
  --docs-dir   "${DOCS_DIR}"   \
  --header-dir "${HEADER_DIR}" \
  "$@"
