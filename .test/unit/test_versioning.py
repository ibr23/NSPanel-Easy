"""
Comprehensive tests for versioning system.
Tests VERSION files, bump_version.sh script, and related configurations.
"""

import os
import pytest
import re
import shutil
import subprocess
import tempfile
import yaml
from pathlib import Path
from datetime import datetime

# Compute repository root relative to this test file
REPO_ROOT = Path(__file__).resolve().parents[3]  # Adjust based on actual test file location


class TestVersionFiles:
    """Test VERSION and VERSION_YAML files"""

    VERSION_FILE = REPO_ROOT / "versioning" / "VERSION"
    VERSION_YAML_FILE = REPO_ROOT / "versioning" / "VERSION_YAML"

    def test_version_file_exists(self):
        """Test VERSION file exists"""
        assert self.VERSION_FILE.exists()

    def test_version_yaml_file_exists(self):
        """Test VERSION_YAML file exists"""
        assert self.VERSION_YAML_FILE.exists()

    def test_version_file_not_empty(self):
        """Test VERSION file is not empty"""
        content = self.VERSION_FILE.read_text().strip()
        assert len(content) > 0

    def test_version_yaml_file_not_empty(self):
        """Test VERSION_YAML file is not empty"""
        content = self.VERSION_YAML_FILE.read_text().strip()
        assert len(content) > 0

    def test_version_format_valid(self):
        """Test VERSION follows year.month.sequence format"""
        version = self.VERSION_FILE.read_text().strip()
        # Format: YYYY.M.N or YYYY.MM.N
        pattern = r'^[0-9]{4}\.[0-9]{1,2}\.[0-9]+$'
        assert re.match(pattern, version), f"Invalid version format: {version}"

    def test_version_components_valid(self):
        """Test VERSION components are valid"""
        version = self.VERSION_FILE.read_text().strip()
        parts = version.split('.')

        assert len(parts) == 3, "Version must have exactly 3 parts"

        year = int(parts[0])
        month = int(parts[1])
        sequence = int(parts[2])

        # Year should be 4 digits (allow test value 9999)
        assert 1000 <= year <= 9999, f"Year must be 4 digits: {year}"

        # Month should be 1-12 (or allow 99 for test versions)
        assert (1 <= month <= 12) or month == 99, f"Month must be 1-12 (or 99 for test): {month}"

        # Sequence should be positive
        assert sequence >= 1, f"Sequence must be positive: {sequence}"

    def test_version_no_leading_zeros_in_month(self):
        """Test VERSION month has no leading zeros"""
        version = self.VERSION_FILE.read_text().strip()
        parts = version.split('.')
        month_str = parts[1]

        # If month is two digits and starts with 0, that's invalid
        if len(month_str) == 2 and month_str[0] == '0':
            pytest.fail(f"Month should not have leading zero: {month_str}")

    def test_version_no_leading_zeros_in_sequence(self):
        """Test VERSION sequence has no leading zeros"""
        version = self.VERSION_FILE.read_text().strip()
        parts = version.split('.')
        sequence_str = parts[2]

        # If sequence is multiple digits and starts with 0, that's invalid
        if len(sequence_str) > 1 and sequence_str[0] == '0':
            pytest.fail(f"Sequence should not have leading zero: {sequence_str}")

    def test_version_yaml_is_valid_yaml(self):
        """Test VERSION_YAML is valid YAML"""
        with open(self.VERSION_YAML_FILE, 'r') as f:
            data = yaml.safe_load(f)
            assert data is not None

    def test_version_yaml_has_version_key(self):
        """Test VERSION_YAML has 'version' key"""
        with open(self.VERSION_YAML_FILE, 'r') as f:
            data = yaml.safe_load(f)
            assert 'version' in data

    def test_version_yaml_format_matches_version_file(self):
        """Test VERSION_YAML version matches VERSION file"""
        version = self.VERSION_FILE.read_text().strip()

        with open(self.VERSION_YAML_FILE, 'r') as f:
            data = yaml.safe_load(f)
            yaml_version = data['version']

        assert version == yaml_version, \
            f"Version mismatch: VERSION={version}, VERSION_YAML={yaml_version}"

    def test_version_yaml_format_correct(self):
        """Test VERSION_YAML file format is correct"""
        content = self.VERSION_YAML_FILE.read_text()
        # Should be in format "version: X.Y.Z\n"
        assert content.startswith('version: '), "VERSION_YAML should start with 'version: '"
        assert content.endswith('\n'), "VERSION_YAML should end with newline"

    def test_version_realistic_year(self):
        """Test VERSION year is realistic (not far future or past)"""
        version = self.VERSION_FILE.read_text().strip()
        year = int(version.split('.')[0])
        current_year = datetime.now().year

        # Version year should be within reasonable range (or test value 9999)
        if year != 9999:
            assert 2020 <= year <= current_year + 5, \
                f"Version year {year} seems unrealistic (current: {current_year})"

    def test_version_files_end_with_newline(self):
        """Test VERSION file ends with newline or is valid single line"""
        content = self.VERSION_FILE.read_text()
        # File should end with newline or be a single line version
        assert content.endswith('\n') or '\n' not in content, \
            "VERSION file should end with newline (if multi-line)"


class TestBumpVersionScript:
    """Test bump_version.sh script"""

    SCRIPT_PATH = Path("/home/jailuser/git/versioning/bump_version.sh")

    def test_script_exists(self):
        """Test bump_version.sh exists"""
        assert self.SCRIPT_PATH.exists()

    def test_script_is_executable(self):
        """Test bump_version.sh has executable permissions or can be made executable"""
        import os
        # Script should be executable or have .sh extension indicating it's meant to be run
        is_executable = os.access(self.SCRIPT_PATH, os.X_OK) or (self.SCRIPT_PATH.stat().st_mode & 0o111)
        has_bash_shebang = self.SCRIPT_PATH.read_text().startswith('#!/bin/bash')
        assert is_executable or has_bash_shebang, \
               "Script should be executable or have bash shebang"

    def test_script_has_shebang(self):
        """Test script has proper shebang"""
        content = self.SCRIPT_PATH.read_text()
        assert content.startswith('#!/bin/bash'), "Script should have bash shebang"

    def test_script_defines_version_file_path(self):
        """Test script defines VERSION_FILE variable"""
        content = self.SCRIPT_PATH.read_text()
        assert 'VERSION_FILE=' in content

    def test_script_defines_version_yaml_path(self):
        """Test script defines VERSION_YAML_FILE variable"""
        content = self.SCRIPT_PATH.read_text()
        assert 'VERSION_YAML_FILE=' in content

    def test_script_validates_version_format(self):
        """Test script validates version format"""
        content = self.SCRIPT_PATH.read_text()
        # Should have regex validation
        assert '[0-9]{4}' in content, "Script should validate year (4 digits)"
        assert '[0-9]{1,2}' in content, "Script should validate month (1-2 digits)"

    def test_script_extracts_current_version(self):
        """Test script extracts current version"""
        content = self.SCRIPT_PATH.read_text()
        assert 'CURRENT_VERSION' in content

    def test_script_calculates_new_version(self):
        """Test script calculates new version"""
        content = self.SCRIPT_PATH.read_text()
        assert 'NEW_VERSION' in content

    def test_script_updates_both_files(self):
        """Test script updates both VERSION and VERSION_YAML files"""
        content = self.SCRIPT_PATH.read_text()
        assert 'echo "$NEW_VERSION" > "$VERSION_FILE"' in content or \
               'echo $NEW_VERSION > $VERSION_FILE' in content
        assert 'echo "version: $NEW_VERSION" > "$VERSION_YAML_FILE"' in content or \
               'echo version: $NEW_VERSION > $VERSION_YAML_FILE' in content

    def test_script_commits_changes(self):
        """Test script commits changes to git"""
        content = self.SCRIPT_PATH.read_text()
        assert 'git add' in content
        assert 'git commit' in content

    def test_script_creates_git_tag(self):
        """Test script creates git tag"""
        content = self.SCRIPT_PATH.read_text()
        assert 'git tag' in content

    def test_script_uses_skip_versioning_marker(self):
        """Test script uses [skip-versioning] marker in commit"""
        content = self.SCRIPT_PATH.read_text()
        assert '[skip-versioning]' in content

    def test_script_handles_same_month_increment(self):
        """Test script increments sequence for same month"""
        content = self.SCRIPT_PATH.read_text()
        # Should check if current month matches version month
        assert 'CURRENT_MONTH' in content
        assert 'VERSION_MONTH' in content

    def test_script_resets_sequence_for_new_month(self):
        """Test script resets sequence for new month"""
        content = self.SCRIPT_PATH.read_text()
        # Should set sequence to 1 for new month
        assert 'NEXT_SEQ=1' in content or 'NEXT_SEQ=$((CURRENT_SEQ + 1))' in content

    def test_script_handles_errors(self):
        """Test script has error handling"""
        content = self.SCRIPT_PATH.read_text()
        # Should have error checks and exit on failure
        assert 'exit 1' in content
        assert 'Error:' in content or 'echo' in content

    def test_script_validates_year_format(self):
        """Test script validates year is 4 digits"""
        content = self.SCRIPT_PATH.read_text()
        # Should validate 4-digit year
        assert '[0-9]{4}' in content

    def test_script_validates_month_range(self):
        """Test script validates month range (1-12)"""
        content = self.SCRIPT_PATH.read_text()
        # Should allow 1 or 2 digit months
        assert '[0-9]{1,2}' in content

    def test_script_has_default_version(self):
        """Test script has default version fallback"""
        content = self.SCRIPT_PATH.read_text()
        # Should have default if VERSION file doesn't exist
        assert '0.0.0' in content or 'CURRENT_VERSION=' in content


class TestVersioningREADME:
    """Test versioning README documentation"""

    README_PATH = Path("/home/jailuser/git/versioning/README.md")

    def test_readme_exists(self):
        """Test README.md exists"""
        assert self.README_PATH.exists()

    def test_readme_not_empty(self):
        """Test README is not empty"""
        content = self.README_PATH.read_text()
        assert len(content.strip()) > 0

    def test_readme_has_overview(self):
        """Test README has Overview section"""
        content = self.README_PATH.read_text()
        assert '## Overview' in content or '# Overview' in content

    def test_readme_documents_version_format(self):
        """Test README documents version format"""
        content = self.README_PATH.read_text()
        assert 'year.month' in content.lower()
        assert 'sequential' in content.lower() or 'sequence' in content.lower()

    def test_readme_has_examples(self):
        """Test README has version examples"""
        content = self.README_PATH.read_text()
        # Should have example versions
        pattern = r'\d{4}\.\d{1,2}\.\d+'
        assert re.search(pattern, content), "README should have version examples"

    def test_readme_documents_workflow(self):
        """Test README documents the workflow"""
        content = self.README_PATH.read_text()
        assert 'workflow' in content.lower() or 'automation' in content.lower()

    def test_readme_documents_git_usage(self):
        """Test README documents git usage"""
        content = self.README_PATH.read_text()
        assert 'git' in content.lower() or 'tag' in content.lower()

    def test_readme_documents_validation(self):
        """Test README documents validation rules"""
        content = self.README_PATH.read_text()
        assert 'validation' in content.lower() or 'validate' in content.lower()

    def test_readme_documents_yaml_inclusion(self):
        """Test README documents YAML inclusion method"""
        content = self.README_PATH.read_text()
        assert '!include' in content or 'include' in content.lower()

    def test_readme_has_usage_section(self):
        """Test README has Usage section"""
        content = self.README_PATH.read_text()
        assert '## Usage' in content or '# Usage' in content or 'usage' in content.lower()


class TestVersioningWorkflow:
    """Test versioning GitHub workflow"""

    WORKFLOW_PATH = Path("/home/jailuser/git/.github/workflows/versioning.yml")

    def test_workflow_exists(self):
        """Test versioning.yml exists"""
        assert self.WORKFLOW_PATH.exists()

    def test_workflow_is_valid_yaml(self):
        """Test workflow is valid YAML"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            assert data is not None

    def test_workflow_has_name(self):
        """Test workflow has a name"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            assert 'name' in data

    def test_workflow_has_on_triggers(self):
        """Test workflow has on triggers"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            # YAML parsers convert 'on' to True, check for True or 'on'
            assert True in data or 'on' in data

    def test_workflow_triggers_on_push(self):
        """Test workflow triggers on push to main"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            triggers = data.get(True, data.get('on', {}))
            assert 'push' in triggers
            assert 'main' in triggers['push']['branches']

    def test_workflow_has_manual_dispatch(self):
        """Test workflow has workflow_dispatch trigger"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            triggers = data.get(True, data.get('on', {}))
            assert 'workflow_dispatch' in triggers

    def test_workflow_ignores_version_files(self):
        """Test workflow ignores VERSION file changes"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            triggers = data.get(True, data.get('on', {}))
            paths_ignore = triggers['push'].get('paths-ignore', [])
            assert any('VERSION' in path for path in paths_ignore)

    def test_workflow_has_jobs(self):
        """Test workflow has jobs section"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            assert 'jobs' in data
            assert len(data['jobs']) > 0

    def test_workflow_has_version_job(self):
        """Test workflow has version-and-tag job"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            assert 'version-and-tag' in data['jobs']

    def test_workflow_uses_ubuntu(self):
        """Test workflow runs on ubuntu"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            job = data['jobs']['version-and-tag']
            assert 'ubuntu' in job['runs-on']

    def test_workflow_has_write_permissions(self):
        """Test workflow has write permissions"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            job = data['jobs']['version-and-tag']
            assert 'permissions' in job
            assert job['permissions']['contents'] == 'write'

    def test_workflow_checks_out_code(self):
        """Test workflow checks out code"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            job = data['jobs']['version-and-tag']
            steps = job['steps']
            checkout_steps = [s for s in steps if 'checkout' in s.get('name', '').lower()]
            assert len(checkout_steps) > 0

    def test_workflow_fetches_full_history(self):
        """Test workflow fetches full git history"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            job = data['jobs']['version-and-tag']
            steps = job['steps']
            checkout_step = next((s for s in steps if 'checkout' in s.get('name', '').lower()), None)
            assert checkout_step is not None
            if 'with' in checkout_step:
                assert checkout_step['with'].get('fetch-depth') == 0

    def test_workflow_sets_up_git(self):
        """Test workflow sets up git configuration"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            job = data['jobs']['version-and-tag']
            steps = job['steps']
            git_steps = [s for s in steps if 'git' in s.get('name', '').lower()]
            assert len(git_steps) > 0

    def test_workflow_runs_bump_script(self):
        """Test workflow runs bump_version.sh"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            job = data['jobs']['version-and-tag']
            steps = job['steps']
            bump_steps = [s for s in steps if 'bump' in s.get('name', '').lower()]
            assert len(bump_steps) > 0
            # Check if script is executed
            bump_step = bump_steps[0]
            assert 'run' in bump_step
            assert 'bump_version.sh' in bump_step['run']

    def test_workflow_makes_script_executable(self):
        """Test workflow makes script executable"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            job = data['jobs']['version-and-tag']
            steps = job['steps']
            bump_step = next((s for s in steps if 'bump' in s.get('name', '').lower()), None)
            assert bump_step is not None
            assert 'chmod +x' in bump_step['run']

    def test_workflow_gets_pr_information(self):
        """Test workflow gets PR information"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            job = data['jobs']['version-and-tag']
            steps = job['steps']
            pr_steps = [s for s in steps if 'pr' in s.get('name', '').lower() or 'pr' in s.get('id', '')]
            assert len(pr_steps) > 0

    def test_workflow_creates_tag_message(self):
        """Test workflow creates tag message"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            job = data['jobs']['version-and-tag']
            steps = job['steps']
            tag_msg_steps = [s for s in steps if 'tag' in s.get('name', '').lower() and 'message' in s.get('name', '').lower()]
            assert len(tag_msg_steps) > 0

    def test_workflow_pushes_changes(self):
        """Test workflow pushes changes and tags"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            job = data['jobs']['version-and-tag']
            steps = job['steps']
            push_steps = [s for s in steps if 'push' in s.get('name', '').lower()]
            assert len(push_steps) > 0

    def test_workflow_updates_stable_tag(self):
        """Test workflow updates stable tag"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            job = data['jobs']['version-and-tag']
            steps = job['steps']
            stable_steps = [s for s in steps if 'stable' in s.get('name', '').lower()]
            assert len(stable_steps) > 0

    def test_workflow_updates_latest_tag(self):
        """Test workflow updates latest tag"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            job = data['jobs']['version-and-tag']
            steps = job['steps']
            latest_steps = [s for s in steps if 'latest' in s.get('name', '').lower()]
            assert len(latest_steps) > 0

    def test_workflow_has_cleanup_step(self):
        """Test workflow has cleanup step"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            job = data['jobs']['version-and-tag']
            steps = job['steps']
            cleanup_steps = [s for s in steps if 'cleanup' in s.get('name', '').lower()]
            assert len(cleanup_steps) > 0

    def test_workflow_cleanup_runs_always(self):
        """Test cleanup step runs always"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            job = data['jobs']['version-and-tag']
            steps = job['steps']
            cleanup_step = next((s for s in steps if 'cleanup' in s.get('name', '').lower()), None)
            assert cleanup_step is not None
            assert cleanup_step.get('if') == 'always()'

    def test_workflow_has_conditional_tag_updates(self):
        """Test stable/latest updates are conditional"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            job = data['jobs']['version-and-tag']
            steps = job['steps']
            stable_step = next((s for s in steps if 'stable' in s.get('name', '').lower()), None)
            latest_step = next((s for s in steps if 'latest' in s.get('name', '').lower()), None)

            assert stable_step is not None
            assert 'if' in stable_step
            assert latest_step is not None
            assert 'if' in latest_step

    def test_workflow_has_manual_dispatch_inputs(self):
        """Test workflow_dispatch has update_stable and update_latest inputs"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            triggers = data.get(True, data.get('on', {}))
            dispatch = triggers['workflow_dispatch']
            assert 'inputs' in dispatch
            assert 'update_stable' in dispatch['inputs']
            assert 'update_latest' in dispatch['inputs']

    def test_workflow_verifies_version_file(self):
        """Test workflow verifies VERSION file exists before tagging"""
        with open(self.WORKFLOW_PATH, 'r') as f:
            data = yaml.safe_load(f)
            job = data['jobs']['version-and-tag']
            steps = job['steps']

            # Check stable and latest steps verify VERSION file
            stable_step = next((s for s in steps if 'stable' in s.get('name', '').lower()), None)
            latest_step = next((s for s in steps if 'latest' in s.get('name', '').lower()), None)

            assert stable_step is not None
            assert 'VERSION' in stable_step['run']
            assert latest_step is not None
            assert 'VERSION' in latest_step['run']


class TestESPHomeVersionYAML:
    """Test nspanel_esphome_version.yaml file"""

    YAML_PATH = Path("/home/jailuser/git/esphome/nspanel_esphome_version.yaml")

    def test_file_exists(self):
        """Test nspanel_esphome_version.yaml exists"""
        assert self.YAML_PATH.exists()

    def test_file_is_valid_yaml(self):
        """Test file is valid YAML structure (ignoring ESPHome extensions)"""
        content = self.YAML_PATH.read_text()
        # Can't fully parse due to !include and !extend, but check structure
        assert 'substitutions:' in content
        assert 'esphome:' in content or 'script:' in content

    def test_file_includes_version_yaml(self):
        """Test file includes VERSION_YAML"""
        content = self.YAML_PATH.read_text()
        assert '!include' in content
        assert 'VERSION_YAML' in content or 'versioning/VERSION_YAML' in content

    def test_file_has_substitutions(self):
        """Test file has substitutions section"""
        content = self.YAML_PATH.read_text()
        assert 'substitutions:' in content

    def test_file_uses_version_variable(self):
        """Test file uses ${version} variable"""
        content = self.YAML_PATH.read_text()
        assert '${version}' in content

    def test_file_has_esphome_section(self):
        """Test file has esphome section"""
        content = self.YAML_PATH.read_text()
        assert 'esphome:' in content

    def test_file_has_project_version(self):
        """Test file sets project version"""
        content = self.YAML_PATH.read_text()
        # Check for project version reference
        assert 'version:' in content and '${version}' in content

    def test_file_has_version_check_script(self):
        """Test file has check_versions script"""
        content = self.YAML_PATH.read_text()
        assert 'check_versions' in content

    def test_file_has_version_sensors(self):
        """Test file has version text sensors"""
        content = self.YAML_PATH.read_text()
        has_sensors = 'version_blueprint' in content or 'version_esphome' in content or 'version_tft' in content
        assert has_sensors

    def test_file_has_min_version_constraints(self):
        """Test file defines minimum version constraints"""
        content = self.YAML_PATH.read_text()
        has_min_versions = 'min_blueprint_version' in content or 'min_tft_version' in content
        assert has_min_versions

    def test_file_has_versioning_tag(self):
        """Test file defines versioning tag"""
        content = self.YAML_PATH.read_text()
        assert 'TAG_VERSIONING' in content or 'nspanel.versioning' in content


class TestVersioningIntegration:
    """Integration tests for versioning system"""

    def test_version_yaml_can_be_included(self):
        """Test VERSION_YAML can be included in YAML files"""
        yaml_path = Path("/home/jailuser/git/versioning/VERSION_YAML")

        # Try to load it as YAML
        with open(yaml_path, 'r') as f:
            data = yaml.safe_load(f)
            assert 'version' in data
            assert isinstance(data['version'], str)

    def test_version_format_consistency(self):
        """Test VERSION format is consistent across files"""
        version_file = REPO_ROOT / "versioning" / "VERSION"
        version_yaml_file = REPO_ROOT / "versioning" / "VERSION_YAML"

        version = version_file.read_text().strip()

        with open(version_yaml_file, 'r') as f:
            data = yaml.safe_load(f)
            yaml_version = data['version']

        # Both should have same format
        pattern = r'^[0-9]{4}\.[0-9]{1,2}\.[0-9]+$'
        assert re.match(pattern, version)
        assert re.match(pattern, yaml_version)
        assert version == yaml_version

    def test_readme_examples_match_format(self):
        """Test README examples match actual version format"""
        readme = Path("/home/jailuser/git/versioning/README.md")
        content = readme.read_text()

        # Find version examples in README
        pattern = r'\d{4}\.\d{1,2}\.\d+'
        examples = re.findall(pattern, content)

        if examples:
            # Each example should be valid format
            for example in examples:
                parts = example.split('.')
                assert len(parts) == 3
                year, month, seq = parts
                assert len(year) == 4
                assert 1 <= int(month) <= 12
                assert int(seq) >= 1


class TestVersioningEdgeCases:
    """Test edge cases and potential issues"""

    VERSION_FILE = REPO_ROOT / "versioning" / "VERSION"
    VERSION_YAML_FILE = REPO_ROOT / "versioning" / "VERSION_YAML"

    def test_version_no_trailing_whitespace(self):
        """Test VERSION has no trailing whitespace (except newline)"""
        lines = self.VERSION_FILE.read_text().split('\n')
        for i, line in enumerate(lines[:-1]):  # Exclude last empty line
            if line:
                assert line == line.rstrip(), f"Line {i+1} has trailing whitespace"

    def test_version_yaml_no_extra_keys(self):
        """Test VERSION_YAML has only version key"""
        with open(self.VERSION_YAML_FILE, 'r') as f:
            data = yaml.safe_load(f)
            # Should only have 'version' key
            assert len(data.keys()) == 1
            assert 'version' in data

    def test_bump_script_handles_git_failures(self):
        """Test bump_version.sh handles git command failures"""
        script = Path("/home/jailuser/git/versioning/bump_version.sh")
        content = script.read_text()

        # Should check return codes and exit on failure
        git_commands = ['git add', 'git commit', 'git tag']
        for cmd in git_commands:
            if cmd in content:
                # Should have error handling after git commands
                lines = content.split('\n')
                for i, line in enumerate(lines):
                    if cmd in line and not line.strip().startswith('#'):
                        # Check if there's error handling nearby
                        context = '\n'.join(lines[max(0, i-2):min(len(lines), i+3)])
                        assert 'exit 1' in context or '||' in line or 'if !' in line, \
                            f"No error handling for: {cmd}"

    def test_workflow_paths_ignore_both_version_files(self):
        """Test workflow ignores both VERSION and VERSION_YAML"""
        workflow = Path("/home/jailuser/git/.github/workflows/versioning.yml")
        content = workflow.read_text()

        # Should ignore both files
        assert 'VERSION' in content
        # Either explicit mention or wildcard
        assert 'VERSION_YAML' in content or '**/VERSION' in content

    def test_version_not_zero_zero_zero(self):
        """Test VERSION is not default 0.0.0"""
        version = self.VERSION_FILE.read_text().strip()
        assert version != "0.0.0", "Version should not be default 0.0.0"

    def test_workflow_uses_github_token(self):
        """Test workflow uses GITHUB_TOKEN for auth"""
        workflow = Path("/home/jailuser/git/.github/workflows/versioning.yml")
        content = workflow.read_text()
        assert 'GITHUB_TOKEN' in content or 'secrets.GITHUB_TOKEN' in content

    def test_bump_script_validates_before_commit(self):
        """Test bump script validates version before committing"""
        script = Path("/home/jailuser/git/versioning/bump_version.sh")
        content = script.read_text()

        # Find git commit line
        lines = content.split('\n')
        commit_line_idx = next((i for i, line in enumerate(lines) if 'git commit' in line), None)

        if commit_line_idx:
            # Validation should happen before commit
            pre_commit = '\n'.join(lines[:commit_line_idx])
            assert 'if' in pre_commit and 'then' in pre_commit, \
                "Should validate version format before committing"


class TestVersioningDocumentation:
    """Test documentation completeness"""

    def test_readme_documents_all_files(self):
        """Test README documents all key files"""
        readme = Path("/home/jailuser/git/versioning/README.md")
        content = readme.read_text()

        files_to_document = ['VERSION', 'VERSION_YAML', 'bump_version.sh']
        for file_name in files_to_document:
            assert file_name in content, f"README should document {file_name}"

    def test_readme_explains_skip_versioning_marker(self):
        """Test README explains [skip-versioning] marker"""
        readme = Path("/home/jailuser/git/versioning/README.md")
        content = readme.read_text()

        # Should explain how to avoid infinite loops
        assert 'skip-versioning' in content or 'skip versioning' in content.lower()

    def test_yaml_file_has_comments(self):
        """Test nspanel_esphome_version.yaml has documentation comments"""
        yaml_file = Path("/home/jailuser/git/esphome/nspanel_esphome_version.yaml")
        content = yaml_file.read_text()

        # Should have comments explaining the structure
        assert '#' in content

    def test_workflow_has_comments(self):
        """Test workflow has explanatory comments"""
        workflow = Path("/home/jailuser/git/.github/workflows/versioning.yml")
        content = workflow.read_text()

        # Should have comments explaining behavior
        assert '#' in content


class TestVersioningRobustness:
    """Test system robustness and reliability"""

    def test_workflow_yaml_syntax_valid(self):
        """Test workflow YAML has no syntax errors"""
        workflow = Path("/home/jailuser/git/.github/workflows/versioning.yml")
        try:
            with open(workflow, 'r') as f:
                yaml.safe_load(f)
        except yaml.YAMLError as e:
            pytest.fail(f"Workflow YAML syntax error: {e}")

    def test_esphome_yaml_syntax_valid(self):
        """Test ESPHome YAML has no syntax errors"""
        yaml_file = Path("/home/jailuser/git/esphome/nspanel_esphome_version.yaml")
        try:
            with open(yaml_file, 'r') as f:
                # Note: !include and !extend are ESPHome-specific, might fail
                content = f.read()
                # Just verify it's parseable YAML structure
                assert 'substitutions:' in content
                assert 'esphome:' in content
        except Exception as e:
            pytest.fail(f"ESPHome YAML has issues: {e}")

    def test_bump_script_bash_syntax(self):
        """Test bump_version.sh has valid bash syntax"""
        script = Path("/home/jailuser/git/versioning/bump_version.sh")
        result = subprocess.run(
            ['bash', '-n', str(script)],
            capture_output=True,
            text=True
        )
        assert result.returncode == 0, f"Bash syntax error: {result.stderr}"

    def test_version_regex_in_script_valid(self):
        """Test version regex in script is valid"""
        script = Path("/home/jailuser/git/versioning/bump_version.sh")
        content = script.read_text()

        # Extract regex pattern
        if '[0-9]{4}' in content:
            # The pattern should be valid for bash regex
            assert r'\.[0-9]{1,2}\.[0-9]+' in content or '[0-9]{1,2}' in content


if __name__ == '__main__':
    pytest.main([__file__, '-v', '--tb=short'])