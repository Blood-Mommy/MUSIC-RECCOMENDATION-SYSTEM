# Data Directory

This directory is for storing datasets used by the Music Recommendation System.

## Guidelines

- **DO NOT** commit large data files to the repository
- Add data file patterns to `.gitignore`
- Document your data sources here
- Provide scripts to download or generate required data

## Data Sources

Add links to your datasets here:

### Example Datasets
- [Million Song Dataset](http://millionsongdataset.com/)
- [Spotify Dataset](https://www.kaggle.com/datasets)
- [Last.fm Dataset](https://www.last.fm/api)

## Data Format

Document the expected format of your data files:

```
data/
├── songs.csv           # Song metadata
├── user_preferences.csv # User listening history
└── features.csv        # Audio features
```

## Data Preprocessing

If you have data preprocessing scripts, place them in `src/` and document their usage here.

### Example
```bash
python src/preprocess_data.py --input raw_data.csv --output data/processed.csv
```
