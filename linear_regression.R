data = read.csv('C:\\Users\\arjun\\OneDrive\\Desktop\\Random\\Math Club\\Sublime App\\lasso_data.dat')
summary(data)

model <- lm(class ~ x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, data=data)
