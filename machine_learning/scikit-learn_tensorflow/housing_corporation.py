
# coding: utf-8

# In[9]:


import os
import tarfile
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os
import tarfile
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import hashlib #to create unique train & test split instances
from sklearn.model_selection import StratifiedShuffleSplit
from six.moves import urllib

DOWNLOAD_ROOT='https://raw.githubusercontent.com/ageron/handson-ml/master/'
HOUSING_PATH='datasets/housing'
HOUSING_URL=DOWNLOAD_ROOT + HOUSING_PATH + '/housing.tgz'

# download housing data
def fetch_housing_data(housing_url=HOUSING_URL, housing_path=HOUSING_PATH):
    if not os.path.isdir(housing_path):
        os.makedirs(housing_path)
    tgz_path = os.path.join(housing_path, 'housing.tgz')
    urllib.request.urlretrieve(housing_url, tgz_path)
    housing_tgz = tarfile.open(tgz_path)
    housing_tgz.extractall(path=housing_path)
    housing_tgz.close()

# load housing data in panda DataFrame
def load_housing_data(housing_path=HOUSING_PATH):
    csv_path = os.path.join(housing_path, 'housing.csv')
    return pd.read_csv(csv_path)

#creates a unique hash
def test_set_check(identifier, test_ratio, hash):
    return hash(np.int64(identifier)).digest()[-1] < 256 * test_ratio

def split_train_test_by_id(data, test_ratio, id_column, hash=hashlib.md5):
    ids = data[id_column]
    in_test_set = ids.apply(lambda id_:test_set_check(id_,test_ratio, hash))
    return data.loc[~in_test_set], data.loc[in_test_set]

# create a test set
def split_train_set(data, test_ratio):
    shuffled_indices = np.random.permutation(len(data))
    test_set_size = int(len(data) * test_ratio)
    test_indices = shuffled_indices[:test_set_size]
    train_indices = shuffled_indices[test_set_size:]
    return data.iloc[train_indices], data.iloc[test_indices]

from sklearn.base import BaseEstimator, TransformerMixin
rooms_ix, bedroom_ix, population_ix, household_ix = 3,4,5,6

#Custom Transformer
class CombinedAttributesAdder(BaseEstimator, TransformerMixin):
    def __init__(self, add_bedrooms_per_room = True): #no *args or **kwargs to get methods: get_params, set_params
        self.add_bedrooms_per_room = add_bedrooms_per_room
    def fit(self, X, y=None):
        return self
    def transform(self, X, y=None):
        rooms_per_household = X[:, rooms_ix]/X[:, household_ix]
        population_per_household = X[:, bedroom_ix] / X[:, rooms_ix]
        if self.add_bedrooms_per_room:
            bedrooms_per_room = X[:, bedroom_ix] / X[:, rooms_ix]
            return np.c_[X, rooms_per_household, population_per_household, bedrooms_per_room]
        else:
            return np.c_[X, rooms_per_household, population_per_household]

#Allows to feed a Panda DataFrame directly into the pipeline instead of extracting numerical columns into Numpy array
class DataFrameSelector(BaseEstimator, TransformerMixin):
    def __init__(self, attribute_names):
        self.attribute_names = attribute_names
    def fit(self, X, y=None):
        return self
    def transform(self, X):
        return X[self.attribute_names].values
#fetch_housing_data()

print('Start')
housing = load_housing_data()

#stratifying the data 
housing['income_cat'] = np.ceil(housing['median_income']/1.5)
housing['income_cat'].where(housing['income_cat'] < 5, 5.0, inplace=True)
#data description
#housing.info()

#get distinct catego;ries
#housing['ocean_proximity'].value_counts()

#get summary of numerical attributes
#housing.describe()

# Plot histogram of each numerical attribute
# housing.hist(bins=50, figsize=(20,15))
# plt.show()

# train_set, test_set = split_train_set(housing, 0.2)

# print('train: {0}, test: {1}'.format(len(train_set), len(test_set)))

housing_with_ids = housing.reset_index()
train_set, test_set = split_train_test_by_id(housing_with_ids, 0.2, 'index')
split = StratifiedShuffleSplit(n_splits=1, test_size=0.2, random_state=42)

strat_train_set = strat_test_set = None

for train_index, test_index in split.split(housing, housing['income_cat']):
    strat_train_set = housing.loc[train_index]
    strat_test_set = housing.loc[test_index]
    
for set_ in (strat_train_set, strat_test_set):
    set_.drop('income_cat', axis=1, inplace=True)

housing = strat_train_set.copy()
#s = district's populatin
#c = color - represents the price

# housing.plot(kind='scatter', x='longitude', y='latitude', alpha=0.4,
#             s=housing['population']/100, label='population', figsize=(10,7),
#             c='median_house_value', cmap=plt.get_cmap('jet'), colorbar=True,)
#plt.legend()
    
#calculating standard correlation coefficient between every pair of attributes
# corr_matrix = housing.corr()
# corr_matrix['median_house_value'].sort_values(ascending=False)
# housing['income_cat'].value_counts()/len(housing)

#Visualizing standard correlation
# from pandas.plotting import scatter_matrix
# attributes = ['median_house_value', 'median_income', 'total_rooms', 'housing_median_age']
# scatter_matrix(housing[attributes], figsize=(12, 8))

#housing.plot(kind='scatter', x='median_income', y='median_house_value', alpha=0.1)

#Combining interesting attributes
housing['rooms_per_household'] = housing['total_rooms']/housing['households']
housing['bedrooms_per_room'] = housing['total_bedrooms']/housing['total_rooms']
housing['population_per_household'] = housing['population']/housing['households']

corr_matrix = housing.corr()
#corr_matrix['median_house_value'].sort_values(ascending=False)

#Preparing Data/Clean up
housing = strat_train_set.drop('median_house_value', axis=1)
housing_labels = strat_train_set['median_house_value'].copy()

#Take care of missing values
from sklearn.preprocessing import Imputer
imputer = Imputer(strategy='median')

#since median can only be computed on numerical fields, we drop ocean_proximity object
housing_num = housing.drop('ocean_proximity', axis=1)
#training imputer
imputer.fit(housing_num)

#estimator learned parameter
# imputer.statistics_
#housing_num.median().values

#Using trained (median) imputer to transformed data by replacing missing values
X = imputer.transform(housing_num)

#converting text labels to numbers
from sklearn.preprocessing import LabelEncoder
encoder = LabelEncoder()
housing_cat = housing['ocean_proximity']
housing_cat_encoded = encoder.fit_transform(housing_cat)
#housing_cat_encoded
# print(encoder.classes_)
#Creating a one-hot encoder to avoid misinterpretation of encoded categories
from sklearn.preprocessing import OneHotEncoder
encoder = OneHotEncoder()
housing_cat_1hot = encoder.fit_transform(housing_cat_encoded.reshape(-1,1))

#Shothand to apply both transformations: text categories to integer then integer to one-hot vectors
from sklearn.preprocessing import LabelBinarizer
encoder = LabelBinarizer()
housing_cat_1hot = encoder.fit_transform(housing_cat)
housing_cat_1hot

#Using custom transformer
attr_adder = CombinedAttributesAdder(add_bedrooms_per_room=False)
housing_extra_attribs = attr_adder.transform(housing.values)

#Using Pipeline to order data transformation steps
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler
num_attribs = list(housing_num)
cat_attribs = ['ocean_proximity']

num_pipeline = Pipeline([
    ('selector', DataFrameSelector(num_attribs)),
    ('imputer', Imputer(strategy='median')), 
    ('attribs_adder', CombinedAttributesAdder()),
    ('std_scaler', StandardScaler()),
])

cat_pipeline = Pipeline([
    ('selector', DataFrameSelector(cat_attribs)),
    ('label_binarizer', LabelBinarizer()),
])

from sklearn.pipeline import FeatureUnion
full_pipeline = FeatureUnion(transformer_list=[
    ('num_pipeline', num_pipeline),
    ('cat_pipeline', cat_pipeline)
])

housing_prepared = full_pipeline.fit_transform(housing)
housing_prepared.shape
# housing_num_tr = num_pipeline.fit_transform(housing_num)

#Training Model #1: LinearRegression
from sklearn.linear_model import LinearRegression
lin_reg = LinearRegression()
lin_reg.fit(housing_prepared, housing_labels)

#Evaluating Model
some_data = housing.iloc[:5]
some_labels = housing_labels.iloc[:5]
some_data_prepared = full_pipeline.transform(some_data)

# print('Predictions: {}'.format(lin_reg.predict(some_data_prepared)))
# print('Labels: {}'.format(list(some_labels)))

from sklearn.metrics import mean_squared_error
housing_predictions = lin_reg.predict(housing_prepared) #model underfits the data
lin_mse = mean_squared_error(housing_labels, housing_predictions)
lin_rmse = np.sqrt(lin_mse)
#lin_rmse

#Training (more powerful) Model #2: DecisionTree
from sklearn.tree import DecisionTreeRegressor
tree_reg = DecisionTreeRegressor()
tree_reg.fit(housing_prepared, housing_labels)
housing_predictions = tree_reg.predict(housing_prepared)
tree_mse = mean_squared_error(housing_labels, housing_predictions)
tree_rmse = np.sqrt(tree_mse)
# tree_rmse


#Cross-Validation K-fold
from sklearn.model_selection import cross_val_score
#Linear Regression
lin_scores = cross_val_score(lin_reg, housing_prepared, housing_labels, scoring='neg_mean_squared_error',
                            cv=10)
lin_rmse_scores = np.sqrt(-lin_scores)

#DecisionTree
scores = cross_val_score(tree_reg, housing_prepared, housing_labels, scoring='neg_mean_squared_error',
                        cv=10)
tree_rmse_scores = np.sqrt(-scores)

def display_scores(name, scores):
    print('-------------------{}-------------------'.format(name))
    print('Scores: {}'.format(scores))
    print('Mean: {}'.format(scores.mean()))
    print('Standard deviation: {}'.format(scores.std()))

#Uinsg Model #3:Forest regressor
from sklearn.ensemble import RandomForestRegressor
forest_reg = RandomForestRegressor()
forest_reg.fit(housing_prepared, housing_labels)
forest_scores = cross_val_score(forest_reg, housing_prepared, housing_labels, scoring='neg_mean_squared_error',
                        cv=10)
forest_rmse_scores = np.sqrt(-forest_scores)

display_scores('Linear Regressor', lin_rmse_scores)
display_scores('DecisionTree Regressor', tree_rmse_scores)
display_scores('RandomForest Regressor', forest_rmse_scores)
                            
param_grid = [
    {'n_estimators': [3, 10, 30],'max_features': [2,4,6,8]},
    {'bootstrap': [False], 'n_estimators': [3,10], 'max_features': [2,3,4]},
]

from sklearn.model_selection import GridSearchCV
forest_reg = RandomForestRegressor()
grid_search = GridSearchCV(forest_reg, param_grid, cv=5, scoring='neg_mean_squared_error')
grid_search.fit(housing_prepared, housing_labels)
cvres = grid_search.cv_results_
print('Best params: {}'.format(grid_search.best_params_))
print('Best estimator: {}'.format(grid_search.best_estimator_))

for mean_score, params in zip(cvres['mean_test_score'], cvres['params']):
    print(np.sqrt(-mean_score), params)

feature_importances = grid_search.best_estimator_.feature_importances_
print('Feature importances: {}'.format(feature_importances))

extra_attribs = ['rooms_per_hhold', 'pop_per_hhold', 'bedrooms_per_room']
cat_one_hot_attribs = list(encoder.classes_)
attributes = num_attribs + extra_attribs + cat_one_hot_attribs
print(sorted(zip(feature_importances, attributes), reverse=True))

#Final Model
final_model = grid_search.best_estimator_
X_test = strat_test_set.drop('median_house_value', axis=1)
y_test = strat_test_set['median_house_value'].copy()
X_test_prepared = full_pipeline.transform(X_test)

final_predictions = final_model.predict(X_test_prepared)
print('Final Predictions: {}'.format(final_predictions))
final_mse = mean_squared_error(y_test, final_predictions)
final_rmse = np.sqrt(final_mse)
print('Final RMSE: {}'.format(final_rmse))

print('Done')

