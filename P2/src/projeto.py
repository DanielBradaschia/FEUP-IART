import sqlite3
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.pyplot import figure 

cnx = sqlite3.connect('../soccer/database.sqlite')
data = pd.read_sql_query("SELECT * FROM Player_Attributes", cnx)

#dropping null vallues
data = data.dropna()

#dropping unecessary or non numeric value collumns
features = list(data.columns[~(data.columns.str.contains('id'))])
features
remove = ['date', 'overall_rating', 'preferred_foot', 'attacking_work_rate', 'defensive_work_rate']
for i in remove:
    features.remove(i)

#correlation plot
x_values = []
y_values = []

# Plotting the values using matplotlib.pyplot
plt.xlabel('Features in the data')
plt.ylabel('Correlation Coefficient with Overall Rating')
plt.title('Correlation of Overall Rating with different features')
plt.yticks([0, 1])

#Adjusting the size of the image 
figure(num = None, figsize = (30, 6), dpi=80, facecolor='w', edgecolor='k')

#Plotting a subplot
fig, axis = plt.subplots(figsize = (40, 8))
# Grid lines, Xticks, Xlabel, Ylabel

axis.yaxis.grid(True)
axis.set_title('Overall Player Rating',fontsize=10)
axis.set_xlabel('Player Features',fontsize=10)   
axis.set_ylabel('Correlation Values',fontsize=10)
axis.set_yticks([0,1])
axis.set_yticklabels(['0', '1'])

axis.plot(x_values, y_values)
plt.show()