# Examples

This directory contains example code and usage demonstrations for the Music Recommendation System.

## Contents

Add example scripts and notebooks here to demonstrate:
- Basic usage of the recommendation system
- Advanced features
- Integration examples
- Tutorial notebooks

## Example Structure

```
examples/
├── basic_usage.py           # Simple usage example
├── advanced_features.py     # Advanced functionality demo
└── notebooks/
    └── tutorial.ipynb       # Jupyter notebook tutorial
```

## Running Examples

```bash
# Run a basic example
python examples/basic_usage.py

# For Jupyter notebooks
jupyter notebook examples/notebooks/tutorial.ipynb
```

## Example Code

```python
# Example: Basic recommendation
from src.recommender import MusicRecommender

# Initialize recommender
recommender = MusicRecommender()

# Get recommendations
recommendations = recommender.get_recommendations(user_id=1, num_songs=10)
print(recommendations)
```
