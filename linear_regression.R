data = read.csv('C:\\Users\\arjun\\OneDrive\\Desktop\\Random\\Math Club\\Sublime App\\iris.csv')
summary(data)

model <- lm(class ~ X5.1, X3.5, X1.4, X0.2, data=data)
