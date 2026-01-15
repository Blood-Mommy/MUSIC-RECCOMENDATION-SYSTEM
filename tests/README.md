# Tests

This directory contains tests for the Music Recommendation System.

## Structure

Organize your tests to mirror your source code structure:

```
tests/
├── test_recommender.py
├── test_data_loader.py
└── test_utils.py
```

## Running Tests

```bash
# Using pytest
pytest tests/

# Using unittest
python -m unittest discover tests/
```

## Writing Tests

Follow these guidelines:
- Name test files with `test_` prefix
- Name test functions with `test_` prefix
- Write clear, descriptive test names
- Include both positive and negative test cases
- Aim for good code coverage

## Example Test

```python
import unittest
from src.recommender import MusicRecommender

class TestMusicRecommender(unittest.TestCase):
    def test_recommendation_generation(self):
        # Your test code here
        pass
```
