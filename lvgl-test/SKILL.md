---
name: lvgl-test
description: LVGL test automation using existing scripts in tests/ and scripts/ directories. Provides tools for compiling, testing, checking code coverage, formatting code, and debugging LVGL test cases. Use when working with LVGL test development, running test suites, verifying code coverage, or debugging test failures in LVGL projects.
---

# LVGL Test Automation

## Overview

Automate LVGL test development workflow using existing scripts in `tests/` and `scripts/` directories for building, testing, coverage analysis, code formatting, and GDB debugging.

## Quick Start

All automation scripts are in `tests/` and `scripts/` directories. Execute them directly from the LVGL root directory.

**Requirements:**
- Python 3.9+
- CMake, GCC, GDB
- `pip install -r scripts/prerequisites-pip.txt`

## Available Scripts

### Run Tests

**Script:** `tests/main.py`

Run LVGL tests with automatic build.

**Usage:**
```bash
python3 tests/main.py test [options]
```

**Options:**
- `--test-suite FILTER`: Filter tests by name, e.g., "arc", "btn", "menu"
- `--report`: Generate test report (default)
- `--clean`: Clean build directory before building

**Examples:**
- Run all tests: `python3 tests/main.py test`
- Run arc tests: `python3 tests/main.py test --test-suite arc`
- Clean build: `python3 tests/main.py test --clean`

### Check Coverage

**Script:** `scripts/check_gcov_coverage.py`

Check code coverage for files or directories.

**Usage:**
```bash
python3 scripts/check_gcov_coverage.py --path <path> [options]
```

**Options:**
- `--path PATH`: File or directory path to check coverage (relative to lvgl directory or absolute path)
- `--fail-under PERCENTAGE`: Fail if coverage below this percentage (default: 0)

**Examples:**
- Check file: `python3 scripts/check_gcov_coverage.py --path src/widgets/lv_btn.c`
- Check directory: `python3 scripts/check_gcov_coverage.py --path src/misc`
- Require 80%: `python3 scripts/check_gcov_coverage.py --path src/core/lv_obj.c --fail-under 80`

**Output:** Coverage percentage and list of uncovered line numbers.

### Format Code

**Script:** `scripts/code-format.py`

Format code with astyle.

**Usage:**
```bash
python3 scripts/code-format.py [target]
```

**Targets:**
- `demos`: Format demos only
- `examples`: Format examples only
- `src`: Format src only
- `tests`: Format tests only
- (no target): Format all

**Examples:**
- Format all: `python3 scripts/code-format.py`
- Format tests only: `python3 scripts/code-format.py tests`

### Debug with GDB

**Script:** `tests/main.py`

Debug tests with GDB.

**Usage:**
```bash
python3 tests/main.py gdb [options]
```

**Options:**
- `--test-name NAME`: Test name like "arc", "btn". If empty, lists available tests
- `--breakpoint BP`: Required. "file.c:123" or "function_name"
- `--commands CMD1,CMD2,...`: GDB commands after breakpoint hit (default: "bt,info locals,continue")
- `--max-hits N`: Maximum breakpoint hits (default: 3)

**Examples:**
- Debug arc test: `python3 tests/main.py gdb --test-name arc --breakpoint test_arc_creation_successful`
- Set line breakpoint: `python3 tests/main.py gdb --test-name btn --breakpoint test_btn.c:50`
- Custom commands: `python3 tests/main.py gdb --test-name label --breakpoint lv_label_set_text --commands "bt,print text,continue"`

## Workflows

### Writing New Tests

1. Write/modify test code in `tests/src/test_cases/widgets/` or appropriate subdirectory
2. Format code: `python3 scripts/code-format.py tests`
3. Run tests: `python3 tests/main.py test --test-suite your_test`
4. Check coverage: `python3 scripts/check_gcov_coverage.py --path src/your_module.c`
5. Read uncovered lines from coverage output
6. Improve test to cover missed branches/conditions
7. Repeat steps 2-6 until coverage target met

### Improving Coverage

1. Check current: `python3 scripts/check_gcov_coverage.py --path src/widgets/lv_btn.c`
2. Read uncovered lines from output
3. Write tests targeting those branches/conditions
4. Rerun: `python3 tests/main.py test --test-suite btn`
5. Verify improvement: `python3 scripts/check_gcov_coverage.py --path ... --fail-under 80`

### Debugging Failures

1. Run failing test: `python3 tests/main.py test --test-suite failing_test`
2. Find FAIL assertion in output
3. Set breakpoint: `python3 tests/main.py gdb --test-name ... --breakpoint ...`
4. Use "print var" in commands to inspect variables
5. Fix and rerun

### Troubleshooting

**If gcov files fail to generate or coverage data seems stale:**
- Run with clean build: `python3 tests/main.py test --test-suite your_test --clean`

**If code logic is unclear from static analysis:**
- Use GDB to step through: `python3 tests/main.py gdb --test-name ... --breakpoint function_or_line`
- Add "step", "next", "print var" to commands for detailed inspection

## Test Development Guidelines

### Test File Structure

Tests live in:
- `tests/src/test_cases/widgets/` - Widget tests (test_arc.c, test_btn.c, etc.)
- `tests/src/test_cases/libs/` - Library tests
- `tests/src/test_cases/draw/` - Drawing tests

### Test Code Template

```c
#include "../lv_test.h"
#include "lvgl.h"

void test_widget_basic(void)
{
    lv_obj_t * obj = lv_widget_create(lv_screen_active());
    lv_widget_set_value(obj, 100);
    TEST_ASSERT_EQUAL(100, lv_widget_get_value(obj));
}

void test_widget_edge_case(void)
{
    lv_obj_t * obj = lv_widget_create(lv_screen_active());
    lv_widget_set_value(obj, INT32_MAX);
    TEST_ASSERT_EQUAL(INT32_MAX, lv_widget_get_value(obj));
}
```

### Assertions

**Basic:**
- `TEST_ASSERT(condition)`
- `TEST_ASSERT_TRUE(condition)`
- `TEST_ASSERT_FALSE(condition)`
- `TEST_ASSERT_NULL(ptr)`
- `TEST_ASSERT_NOT_NULL(ptr)`

**Comparison (use type-specific versions):**
- `TEST_ASSERT_EQUAL(expected, actual)` - for int
- `TEST_ASSERT_EQUAL_UINT(expected, actual)` - for unsigned int
- `TEST_ASSERT_EQUAL_UINT8/16/32(expected, actual)` - for specific unsigned types
- `TEST_ASSERT_EQUAL_INT8/16/32(expected, actual)` - for specific signed types
- `TEST_ASSERT_EQUAL_PTR(expected, actual)` - for pointers
- `TEST_ASSERT_EQUAL_STRING(expected, actual)` - for strings
- `TEST_ASSERT_EQUAL_MEMORY(expected, actual, len)` - for memory blocks

**Important:** Always use the correct type-specific assertion. Using wrong type may cause incorrect comparison results or compiler warnings.

### Test Naming

Use: `test_<module>_<feature>_<scenario>`

Examples:
- `test_arc_creation_successful`
- `test_arc_should_truncate_to_max_range`
- `test_btn_click_event_fired`

### Coverage Targets

- New code: aim for 80%+
- Core modules: aim for 90%+
- Always test if/else branches, switch cases, edge conditions

## Before Commit

Always run:
1. `python3 scripts/code-format.py`
2. `python3 tests/main.py test`
3. `python3 scripts/check_gcov_coverage.py --path src/changed_file.c --fail-under 70`

## Notes

- Test timeout is 600 seconds. Use `--test-suite` to run specific tests if builds are slow
- Coverage data comes from the most recent test run with report enabled
- GDB may show LeakSanitizer errors - this is normal and can be ignored
- Call `tests/main.py gdb` without `--test-name` to list available test executables
- Test executables are located in `tests/build_test_sysheap/`
