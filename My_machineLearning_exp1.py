from sklearn import datasets
from sklearn.svm import LinearSVC
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import accuracy_score
from joblib import dump
# 导入iris数据集
iris = datasets.load_iris()
X = iris.data[:, :2]
y = iris.target

# 数据预处理，标准化
scaler = StandardScaler()
X_s = scaler.fit_transform(X)

# 划分训练集和测试集
X_train, X_test, y_train, y_test = train_test_split(X_s, y, test_size=0.3, random_state=42)

# 训练模型
def TrainModel(p, X_train, X_test, y_train, y_test):
    # 实例化训练器
    svc = LinearSVC(penalty=p, dual=False, random_state=42)
    # 训练集训练
    svc.fit(X_train, y_train)
    # 预测
    y_pred = svc.predict(X_test)
    svc_score = accuracy_score(y_test, y_pred)
    print(f"{p}正则化模型准确率: {svc_score}")
    return svc

# 训练
svc_l1 = TrainModel('l1', X_train, X_test, y_train, y_test)
svc_l2 = TrainModel('l2', X_train, X_test, y_train, y_test)

# 定义绘制决策边界的函数
def plot_decision_boundaries(svc, X, y, title):
    # 创建网格
    x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
    y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
    xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.02),
                         np.arange(y_min, y_max, 0.02))
    
    # 预测网格点的类别
    Z = svc.predict(np.c_[xx.ravel(), yy.ravel()])
    Z = Z.reshape(xx.shape)
    
    # 绘制决策边界
    plt.contour(xx, yy, Z, colors='k', levels=[0.5,1,2], linestyles=['dashed'], linewidths=1)
    
    # 绘制数据点
    plt.scatter(X[:, 0], X[:, 1], c=y, edgecolors='k', s=40, cmap='viridis')
    
    # 添加标题和标签
    plt.title(title, fontsize=16, fontweight='bold')
    plt.xlabel('Sepal length [cm]', fontsize=14)
    plt.ylabel('Sepal width [cm]', fontsize=14)
    
    # 设置图例
    handles = [plt.Line2D([0], [0], marker='o', color='w', label='Class 1', markerfacecolor='black', markersize=10),
               plt.Line2D([0], [0], marker='o', color='w', label='Class 2', markerfacecolor='orange', markersize=10),
               plt.Line2D([0], [0], marker='o', color='w', label='Class 3', markerfacecolor='green', markersize=10)]
    plt.legend(handles=handles, loc='upper right')
    
    # 设置图表的布局
    plt.tight_layout()
    
    # 设置坐标轴的范围
    plt.xlim(x_min, x_max)
    plt.ylim(y_min, y_max)
    
    # 设置坐标轴的刻度大小
    plt.xticks(fontsize=12)
    plt.yticks(fontsize=12)
    
    # 添加网格
    plt.grid(True, linestyle='--', color='gray', linewidth=0.5)
    
    # 显示图表
    plt.show()

# 可视化L1正则化模型的结果
plot_decision_boundaries(svc_l1, X_s[:, :2], y, "L1 Regularization")
# 可视化L2正则化模型的结果
plot_decision_boundaries(svc_l2, X_s[:, :2], y, "L2 Regularization")
# 保存模型
dump(svc_l1, 'svc_l1.joblib')
dump(svc_l2, 'svc_l2.joblib')
