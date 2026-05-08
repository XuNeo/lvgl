# LVGL Agent Guidelines

## Build Commands

### CMake Build
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Test Build with Options
```bash
# From repository root
python tests/main.py build                                    # Standard build
python tests/main.py --build-option=OPTIONS_16BIT build       # 16-bit color depth
python tests/main.py --build-option=OPTIONS_SDL build         # With SDL driver
```

## Test Commands

### Unit Tests (Primary Method)
```bash
# Install prerequisites first (Linux)
./scripts/install-prerequisites.sh

# Run tests
./tests/main.py test                                          # Run all tests
./tests/main.py --clean --report build test                   # Build, test, coverage report
./tests/main.py --update-image test                           # Update reference images
./tests/main.py --help                                        # Full options
```

### Docker Testing
```bash
docker build . -f tests/Dockerfile -t lvgl_test_env
docker run --rm -it -v $(pwd):/work lvgl_test_env "./tests/main.py"
```

### Performance Tests (Requires Docker + Linux)
```bash
./tests/perf.py test
./tests/perf.py --help
```

### Single Test File
Tests are located in `tests/src/test_cases/`. To run a specific test, modify the test runner or use the main.py filtering options.

## Code Style Commands

### Format Code
```bash
# Install astyle if needed
./scripts/install_astyle.sh

# Format source files (run from scripts directory)
cd scripts && python code-format.py
```

### Pre-commit Hooks
```bash
pre-commit install                                            # Enable auto-formatting on commit
pre-commit run format-source                                  # Test formatting without committing
SKIP=format-source git commit                                 # Skip formatting hook
```

### Static Analysis
```bash
./scripts/cppcheck_run.sh                                     # Run cppcheck
./scripts/infer_run.sh                                        # Run Facebook Infer
```

## Code Style Guidelines

### Naming Conventions
- **Variables/functions**: lowercase with underscores (`height_tmp`, `get_width`)
- **Enums/defines**: UPPERCASE (`MAX_LINE_NUM`, `LV_STATE_CHECKED`)
- **API functions**: `lv_<module>_<action>_<subject>` pattern (`lv_button_create`, `lv_label_set_text`)
- **Typedefs**: always end with `_t` (`lv_obj_t`, `lv_event_t`)
- **Private functions**: prefix with underscore (`_lv_internal_func`)

### Allowed Abbreviations
`dsc`, `param`, `indev`, `anim`, `buf`, `str`, `min/max`, `alloc`, `ctrl`, `pos`

### Formatting Rules
- **Indentation**: 4 spaces (no tabs), except Kconfig and Makefiles
- **Max line length**: 120 characters
- **Braces**: K&R style - new line for functions, same line for statements
- **Pointer alignment**: middle (`char * ptr`, not `char* ptr` or `char *ptr`)
- **Operators**: spaces around operators (`x + y`, not `x+y`)

### Variable Declarations
- One declaration per line: `char x; char y;` (not `char x, y;`)
- Use `<stdint.h>` types (`uint8_t`, `int32_t`)
- Declare variables where needed, not at function start
- File-level variables must be `static`
- No global variables - use `lv_global_t` structure

### Comments
- **Doxygen format** for function prototypes in `.h` files
- **Block comments** `/* */` only (no `//` end-of-line comments)
- Space after `/*` and before `*/`
- Explain **why**, not **what**: `x++; /* Point to closing '\0' of string */`
- Use backticks for code elements: `` `variable_name` ``

### Doxygen Comment Template
```c
/**
 * Brief description.
 * @param  parent     description of argument
 * @return   description of return value.
 */
type_name_t * lv_function_name(lv_obj_t * parent);
```

### File Structure (in order)
1. `/*********************\n *      INCLUDES\n *********************/`
2. `/*********************\n *      DEFINES\n *********************/`
3. `/*********************\n *      TYPEDEFS\n *********************/`
4. `/*********************\n *   STATIC PROTOTYPES\n *********************/`
5. `/*********************\n *   STATIC VARIABLES\n *********************/`
6. `/*********************\n *  GLOBAL FUNCTIONS\n *********************/`

Use template files: `src/misc/lv_templ.c` and `src/misc/lv_templ.h`

## API Conventions

- Use `enum` instead of macros for constants
- Array parameters: `type name[]` not `type * name`
- Use typed pointers, not `void *`
- Widget constructor: `lv_<widget>_create(lv_obj_t * parent)`
- Widget functions: receive `lv_obj_t *` as first argument
- Callbacks: pass struct with `void * user_data` or use last argument as `void * user_data`
- Export constants for bindings: `LV_EXPORT_CONST_INT(defined_value)`

## Commit Message Format

```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types**: `fix`, `feat`, `arch`, `perf`, `example`, `docs`, `test`, `chore`
**Example**: `fix(image): update size when a new source is set`

## Error Handling

Use LVGL's assert macros:
```c
LV_ASSERT_MALLOC(ptr);
LV_ASSERT_NULL(ptr);
```

## Configuration

- Copy `lv_conf_template.h` to `lv_conf.h`
- Enable first `#if 0` to `#if 1`
- Adjust settings as needed
- Kconfig also supported for some platforms

## Key Directories

- `src/` - Core library source
- `src/widgets/` - Widget implementations
- `src/misc/` - Utility functions
- `tests/src/test_cases/` - Unit tests
- `demos/` - Demo applications
- `examples/` - Code examples
- `docs/src/` - Documentation source

## Documentation

- Build docs: `cd docs && make` or `python docs/build.py html`
- Reference: https://docs.lvgl.io/master/CODING_STYLE.html
