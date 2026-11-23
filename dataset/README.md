# SYNTHETIC temperature dataset

These matrices are deterministic synthetic input, not real-world observations. Each CSV contains only numeric rows with no header. The field is `100 exp(-((x-.30)^2+(y-.35)^2)/.012) + 65 exp(-((x-.72)^2+(y-.68)^2)/.020)`, with every physical boundary set exactly to zero. Regenerate a size with `python3 dataset/generate_dataset.py --nx N --ny M --output dataset/name.csv`.
