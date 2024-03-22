import numpy as np

# Accuracy scores for each classifier
random_forest_scores = [0.9755102040816327, 0.9714285714285714, 0.9795918367346939, 0.9918032786885246, 0.9549180327868853]
svc_scores = [0.24081632653061225, 0.20816326530612245, 0.24489795918367346, 0.22540983606557377, 0.23770491803278687]
decision_tree_scores = [0.9387755102040817, 0.9387755102040817, 0.9551020408163265, 0.9426229508196722, 0.930327868852459]

# Calculate standard deviation for each classifier
random_forest_std = np.std(random_forest_scores)
svc_std = np.std(svc_scores)
decision_tree_std = np.std(decision_tree_scores)

# Print standard deviation for each classifier
print("Random Forest Standard Deviation:", random_forest_std)
print("SVC Standard Deviation:", svc_std)
print("Decision Tree Standard Deviation:", decision_tree_std)