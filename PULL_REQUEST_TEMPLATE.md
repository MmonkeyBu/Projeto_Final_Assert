<!-- Pull Request Template for Projeto Final -->

## Description
<!-- Describe the changes in this pull request -->

### Type of Change
- [ ] New feature (feat)
- [ ] Bug fix (fix)
- [ ] Documentation update (docs)
- [ ] Code refactoring (refactor)
- [ ] Tests (test)
- [ ] Other: ___________

### Modules Modified
- [ ] Bsp.c / Bsp.h
- [ ] Sampler.c / Sampler.h
- [ ] LedPwm.c / LedPwm.h
- [ ] SerialCmd.c / SerialCmd.h
- [ ] Button.c / Button.h
- [ ] main.c
- [ ] Documentation
- [ ] Other: ___________

### Related Issues
<!-- Link any related issues with #issueNumber -->

---

## Testing

- [ ] Compiled without errors
- [ ] Unit tests pass (if applicable)
- [ ] Integration tests pass
- [ ] No breaking changes
- [ ] Hardware validated (if applicable)

### Testing Checklist
- [ ] ADC sampling (5ms)
- [ ] Filter calculation (500ms)
- [ ] LED PWM control
- [ ] UART commands
- [ ] Button debounce
- [ ] Terminal output format
- [ ] No blocking delays

---

## Documentation

- [ ] Doxygen headers added/updated
- [ ] Function comments updated
- [ ] README updated (if needed)
- [ ] Changelog updated

---

## Commits

All commits follow semantic format: `tipo(escopo): descrição`

Examples:
- `feat(sampler): implement moving average filter`
- `fix(button): correct debounce logic`
- `docs(readme): add integration guide`

---

## Code Review Checklist

- [ ] Code follows Assert standards
- [ ] No magic numbers
- [ ] Proper naming conventions
- [ ] Code sections organized (INCLUDES, DEFINES, etc.)
- [ ] No unnecessary global variables
- [ ] ISRs are short (only flags)
- [ ] Main processing in main loop

---

## Additional Notes
<!-- Add any additional information -->

---

## Performance Impact

- Binary size impact: ________
- RAM usage impact: ________
- Execution time impact: ________

---

**Merged by:** ____________  
**Date:** ____________  
**Tested by:** ____________  

---

*Template for Projeto Final PR - 2026*
