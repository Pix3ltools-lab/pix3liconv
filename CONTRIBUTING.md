# Contributing to Pix3lIconv

First off, thank you for considering contributing to Pix3lIconv! It's people like you that make Pix3lIconv such a great tool.

## Code of Conduct

This project and everyone participating in it is governed by respect and professionalism. By participating, you are expected to uphold this code.

## How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check the existing issues to avoid duplicates. When you are creating a bug report, please include as many details as possible:

**Bug Report Template:**

```markdown
**Describe the bug**
A clear and concise description of what the bug is.

**To Reproduce**
Steps to reproduce the behavior:
1. Go to '...'
2. Click on '...'
3. See error

**Expected behavior**
A clear and concise description of what you expected to happen.

**Screenshots**
If applicable, add screenshots to help explain your problem.

**Environment:**
 - OS: [e.g. Windows 10]
 - Qt Version: [e.g. 6.9.1]
 - Application Version: [e.g. 1.0]

**Additional context**
Add any other context about the problem here.
```

### Suggesting Enhancements

Enhancement suggestions are tracked as GitHub issues. When creating an enhancement suggestion, please include:

- **Use a clear and descriptive title**
- **Provide a step-by-step description** of the suggested enhancement
- **Provide specific examples** to demonstrate the steps
- **Describe the current behavior** and **explain which behavior you expected to see instead**
- **Explain why this enhancement would be useful**

### Pull Requests

1. Fork the repository and create your branch from `master`
2. If you've added code that should be tested, add tests
3. Ensure your code follows the existing style
4. Make sure your code compiles without warnings
5. Update the documentation if needed
6. Write a clear commit message

**Pull Request Template:**

```markdown
**Description**
Brief description of what this PR does.

**Related Issue**
Fixes #(issue number)

**Type of Change**
- [ ] Bug fix (non-breaking change which fixes an issue)
- [ ] New feature (non-breaking change which adds functionality)
- [ ] Breaking change (fix or feature that would cause existing functionality to not work as expected)
- [ ] Documentation update

**Testing**
Describe the tests you ran to verify your changes.

**Checklist**
- [ ] My code follows the style guidelines of this project
- [ ] I have performed a self-review of my own code
- [ ] I have commented my code, particularly in hard-to-understand areas
- [ ] I have made corresponding changes to the documentation
- [ ] My changes generate no new warnings
- [ ] I have tested my changes on Windows
```

## Development Setup

### Prerequisites

- Qt 6.9.1 or later
- MinGW 13.1.0 or compatible compiler
- Git

### Setting Up Your Development Environment

1. **Clone the repository:**
   ```bash
   git clone https://github.com/yourusername/Pix3lIconv.git
   cd Pix3lIconv
   ```

2. **Build in debug mode:**
   ```bash
   qmake CONFIG+=debug
   mingw32-make -j16
   ```

3. **Run the application:**
   ```bash
   ./debug/Pix3lIconv.exe
   ```

## Style Guidelines

### C++ Style Guide

- Use C++17 features
- Follow Qt naming conventions:
  - Classes: `PascalCase`
  - Methods: `camelCase`
  - Member variables: `camelCase`
  - Constants: `UPPER_CASE`
- Use 4 spaces for indentation (no tabs)
- Maximum line length: 120 characters
- Always use `override` keyword for virtual methods
- Use `nullptr` instead of `NULL` or `0`
- Use smart pointers when appropriate

### Commit Message Guidelines

- Use the present tense ("Add feature" not "Added feature")
- Use the imperative mood ("Move cursor to..." not "Moves cursor to...")
- Limit the first line to 72 characters or less
- Reference issues and pull requests liberally after the first line
- Consider starting the commit message with an applicable emoji:
  - 🎨 `:art:` when improving the format/structure of the code
  - 🐛 `:bug:` when fixing a bug
  - ✨ `:sparkles:` when adding a new feature
  - 📝 `:memo:` when writing docs
  - 🚀 `:rocket:` when improving performance
  - ✅ `:white_check_mark:` when adding tests

### Documentation

- Update the README.md if you change functionality
- Comment complex algorithms or non-obvious code
- Use Qt documentation style for C++ comments
- Update TODO.md if you complete tasks or add new ones

## Project Structure

```
Pix3lIconv/
├── main.cpp              # Application entry point
├── mainwindow.h          # Main window header
├── mainwindow.cpp        # Main window implementation
├── pixeliconv.pro        # Qt project file
├── resources.qrc         # Qt resources file
├── resources/            # Resources directory
│   └── icons/           # Icon files
├── README.md            # Project documentation
├── LICENSE              # MIT License
├── CONTRIBUTING.md      # This file
└── TODO.md             # Development tasks
```

## Testing

Before submitting a pull request, please test your changes:

1. Build in both debug and release modes
2. Test all basic functionality:
   - Opening files via browse button
   - Opening files via menu (Ctrl+O)
   - Drag & drop functionality
   - PNG, JPEG, and SVG conversion
   - Preview updates
   - ICO file generation
3. Test edge cases:
   - Invalid file formats
   - Corrupted images
   - Very large images
   - Very small images

## Questions?

Feel free to open an issue with your question or contact the maintainers directly.

## Recognition

Contributors will be recognized in the project's README.md file.

Thank you for contributing to Pix3lIconv! 🎉
