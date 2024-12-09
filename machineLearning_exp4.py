import numpy as np
import matplotlib.pyplot as plt
from sklearn import datasets
from sklearn.svm import LinearSVC
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
import joblib

# 加载数据集
iris = datasets.load_iris()
X = iris.data[:, :2]  # 只使用前两个特征
y = iris.target

# 数据预处理：标准化
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# 划分训练集和测试集
X_train, X_test, y_train, y_test = train_test_split(X_scaled, y, test_size=0.3, random_state=42)

# 训练模型
svc_l1 = LinearSVC(penalty='l1', dual=False, random_state=42).fit(X_train, y_train)
svc_l2 = LinearSVC(penalty='l2', dual=False, random_state=42).fit(X_train, y_train)

# 绘制决策边界的函数
def plot_decision_boundaries(svc, X, y, title):
    x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
    y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
    xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.02),
                         np.arange(y_min, y_max, 0.02))
    Z = svc.predict(np.c_[xx.ravel(), yy.ravel()])
    Z = Z.reshape(xx.shape)

    plt.contourf(xx, yy, Z, alpha=0.8)
    plt.scatter(X[:, 0], X[:, 1], c=y, edgecolors='k', s=20, cmap=plt.cm.Paired)
    plt.title(title)
    plt.xlabel('Sepal length [cm]')
    plt.ylabel('Sepal width [cm]')
    plt.xlim(xx.min(), xx.max())
    plt.ylim(yy.min(), yy.max())
    plt.xticks(())
    plt.yticks(())

# 绘制L1正则化的决策边界
plt.figure(figsize=(12, 6))
plt.subplot(1, 2, 1)
plot_decision_boundaries(svc_l1, X_scaled, y, "L1 Regularization")
plt.subplot(1, 2, 2)
plot_decision_boundaries(svc_l2, X_scaled, y, "L2 Regularization")
plt.tight_layout()
plt.show()

# 保存模型
joblib.dump(svc_l1, 'svc_l1_model.pkl')
joblib.dump(svc_l2, 'svc_l2_model.pkl')
