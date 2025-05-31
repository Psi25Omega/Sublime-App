library(ggplot)

data = read.csv('C:\\Users\\arjun\\OneDrive\\Desktop\\Random\\Math Club\\Sublime App\\r_regression.csv')
summary(data)

model <- lm(y ~ x, data=data)