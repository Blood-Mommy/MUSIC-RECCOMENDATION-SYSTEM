# Contributing to Music Recommendation System

Thank you for your interest in contributing to this project! This document provides guidelines for contributing to the Music Recommendation System.

## 📋 Table of Contents

- [Getting Started](#getting-started)
- [Development Process](#development-process)
- [Code Guidelines](#code-guidelines)
- [Commit Guidelines](#commit-guidelines)
- [Pull Request Process](#pull-request-process)

## 🚀 Getting Started

1. Fork the repository
2. Clone your fork locally
3. Create a new branch for your feature/fix
4. Make your changes
5. Test your changes
6. Submit a pull request

## 💻 Development Process

### Setting Up Development Environment

```bash
# Clone your fork
git clone https://github.com/YOUR_USERNAME/MUSIC-RECCOMENDATION-SYSTEM.git
cd MUSIC-RECCOMENDATION-SYSTEM

# Create virtual environment
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate

# Install dependencies
pip install -r requirements.txt

# Install development dependencies (if any)
pip install pytest pytest-cov black flake8
```

### Making Changes

1. Create a new branch:
```bash
git checkout -b feature/your-feature-name
```

2. Make your changes following the code guidelines
3. Add or update tests as needed
4. Run tests to ensure everything works:
```bash
pytest tests/
```

## 📝 Code Guidelines

### Python Style Guide

- Follow [PEP 8](https://www.python.org/dev/peps/pep-0008/) style guide
- Use meaningful variable and function names
- Maximum line length: 88 characters (Black default)
- Use type hints where appropriate

### Documentation

- Add docstrings to all functions, classes, and modules
- Use Google-style or NumPy-style docstrings
- Update README files when adding new features
- Comment complex logic

### Example Docstring

```python
def recommend_songs(user_id: int, num_recommendations: int = 10) -> list:
    """
    Generate song recommendations for a user.
    
    Args:
        user_id: The ID of the user
        num_recommendations: Number of songs to recommend (default: 10)
    
    Returns:
        A list of recommended song IDs
    
    Raises:
        ValueError: If user_id is invalid
    """
    pass
```

## 📦 Commit Guidelines

### Commit Message Format

```
<type>: <subject>

<body>

<footer>
```

### Types

- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting, etc.)
- `refactor`: Code refactoring
- `test`: Adding or updating tests
- `chore`: Maintenance tasks

### Example

```
feat: Add collaborative filtering algorithm

Implement user-based collaborative filtering for music recommendations.
Uses cosine similarity to find similar users.

Closes #42
```

## 🔄 Pull Request Process

1. Update documentation with details of changes
2. Update the README.md if needed
3. Ensure all tests pass
4. Make sure your code follows the style guidelines
5. Write a clear PR description explaining your changes
6. Link related issues in your PR description

### PR Description Template

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Documentation update
- [ ] Code refactoring

## Testing
Describe how you tested your changes

## Checklist
- [ ] My code follows the style guidelines
- [ ] I have added tests for my changes
- [ ] All tests pass
- [ ] I have updated the documentation
```

## ❓ Questions?

If you have questions, please open an issue with the `question` label.

## 🙏 Thank You!

Your contributions make this project better!
