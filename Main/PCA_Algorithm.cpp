/*
	PCA算法
*/

#include "Computation.h"

PCA_Algorithm::PCA_Algorithm()
{
	eps = 1e-6;
}

ErrNo PCA_Algorithm::PCA_A(double **matrix,const int rows,const int columns,double **&principle,int &k)
{
	//double temp;
	double sum,accumulate;
	int p;
	ErrNo err;
	double **matrix_r = NULL;		//保存相关系数矩阵
	double *eigenvalues = NULL;		//保存特征值
	//double **eigenvectors = NULL;	//保存特征向量
	//double *temp_vector = NULL;		//临时保存数据
	
	// 申请空间
	if(!(matrix_r = new double* [columns]))
		return ALLOCATE_MEMORY_FAIL;
	//if(!(eigenvectors = new double* [columns]))
	//	return ALLOCATE_MEMORY_FAIL;
	for(int i=0;i<columns;i++)
	{
		if(!(matrix_r[i] = new double [columns]))		
			return ALLOCATE_MEMORY_FAIL;
		//if(!(eigenvectors[i] = new double [columns]))
		//	return ALLOCATE_MEMORY_FAIL;
	}
	if(!(eigenvalues = new double [columns]))
		return ALLOCATE_MEMORY_FAIL;
	//if(!(temp_vector = new double [columns]))
	//	return ALLOCATE_MEMORY_FAIL;

	// 测试
	/*
	if(err = InvPowerMeth(matrix,eigenvectors[0],4.2,columns))
		return err;
	for(int j=0;j<columns;j++)
		temp = eigenvectors[0][j];
    */
	
	// 标准化
	if(err = Standardize_Matrix(matrix,rows,columns))
		return err;
	// 求相关系数矩阵
	if(err = Relevance_Matrix(matrix,matrix_r,rows,columns))
		return err;

	// 求特征值
	if(err = QR_Accelerate(matrix_r,eigenvalues,columns))
		return err;

	// 特征值从大到小排列
	QuickSort_LTS(eigenvalues,0,columns-1);

	//求出k的值(根据前k个主成分的得贡献率达到85%)
	sum = 0.0;
	for(int i=0;i<columns;i++)
		sum += eigenvalues[i];
	accumulate = 0.0;
	for(p=0;p<columns;p++)
	{
		accumulate += eigenvalues[p];
		if(accumulate >= (sum * 0.85))
			break;
	}
	k = p+1;

	//为principle申请空间
	if(!(principle = new double* [k]))
		return ALLOCATE_MEMORY_FAIL;
	for(int i=0;i<k;i++)
	{
		if(!(principle[i] = new double [columns]))
			return ALLOCATE_MEMORY_FAIL;
	}

	// 求解特征向量(前k个特征值对应的特征向量)
	for(int i=0;i<k;i++)
	{
		//前面计算出的特征值要放大一点,用原始的会出错
		if(err = InvPowerMeth(matrix_r,principle[i],eigenvalues[i]+0.2,columns))
			return err;
	}

	// 销毁空间
	for(int i=0;i<columns;i++)
	{
		delete [] matrix_r[i];
		//delete [] eigenvectors[i];
	}
	delete [] matrix_r;
	//delete [] eigenvectors;
	delete [] eigenvalues;
	return 0;	
}

ErrNo PCA_Algorithm::PCA_A(double **matrix,const int rows,const int columns,double **matrix_r,double *eigenvalues,double **&principle,int &k)
{
	double sum,accumulate;
	int p;
	ErrNo err;
	//double *temp_vector = NULL;		//临时保存数据
	
	// 申请空间
	//if(!(temp_vector = new double [columns]))
	//	return ALLOCATE_MEMORY_FAIL;
	
	// 标准化
	if(err = Standardize_Matrix(matrix,rows,columns))
		return err;
	// 求相关系数矩阵
	if(err = Relevance_Matrix(matrix,matrix_r,rows,columns))
		return err;

	// 求特征值
	if(err = QR_Accelerate(matrix_r,eigenvalues,columns))
		return err;

	// 特征值从大到小排列
	QuickSort_LTS(eigenvalues,0,columns-1);

	//求出k的值(根据前k个主成分的得贡献率达到85%)
	sum = 0.0;
	for(int i=0;i<columns;i++)
		sum += eigenvalues[i];
	accumulate = 0.0;
	for(p=0;p<columns;p++)
	{
		accumulate += eigenvalues[p];
		if(accumulate >= (sum * 0.85))
			break;
	}
	k = p+1;

	//为principle申请空间
	if(!(principle = new double* [k]))
		return ALLOCATE_MEMORY_FAIL;
	for(int i=0;i<k;i++)
	{
		if(!(principle[i] = new double [columns]))
			return ALLOCATE_MEMORY_FAIL;
	}

	// 求解特征向量(前k个特征值对应的特征向量)
	for(int i=0;i<k;i++)
	{
		//前面计算出的特征值要放大一点,用原始的会出错
		if(err = InvPowerMeth(matrix_r,principle[i],eigenvalues[i]+0.2,columns))
			return err;
	}

	return 0;	
}

ErrNo PCA_Algorithm::Standardize_Matrix(double **matrix,const int rows,const int columns)
{
	double *average = NULL;
	double *standard_diff = NULL;

	//申请临时空间
	if(!(average = new double [columns]))
		return ALLOCATE_MEMORY_FAIL;
	if(!(standard_diff = new double [columns]))
		return ALLOCATE_MEMORY_FAIL;

	// 求矩阵每列的平均数
	for(int j=0;j<columns;j++)
	{
		average[j] = 0.0;
		for(int i=0;i<rows;i++)
		{
			average[j] += matrix[i][j];
		}
		average[j] = average[j] / rows;
	}

	// 求每一列的标准差
	for(int j=0;j<columns;j++)
	{
		standard_diff[j] = 0.0;
		for(int i=0;i<rows;i++)
		{
			standard_diff[j] += (matrix[i][j] - average[j]) * (matrix[i][j] - average[j]);
		}
		if(standard_diff[j] < 0)
			return NEGATIVE_NUMBER;
		standard_diff[j] = sqrt(standard_diff[j] / (rows-1));
	}

	// 标准化
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<columns;j++)
		{
			if(fabs(standard_diff[j]) < eps)
				return DIVIDE_BY_ZERO;
			matrix[i][j] = (matrix[i][j] - average[j]) / standard_diff[j];
		}
	}

	delete [] average;
	delete [] standard_diff;
	return 0;
}

ErrNo PCA_Algorithm::Relevance_Matrix(double **matrix,double **matrix_r,int rows,int columns)
{
	for(int i=0;i<columns;i++)
	{
		for(int j=i;j<columns;j++)
		{
			matrix_r[i][j] = 0.0;
			for(int k=0;k<rows;k++)
			{
				matrix_r[i][j] += matrix[k][i] * matrix[k][j];
			}
			matrix_r[i][j] =matrix_r[i][j] / (rows-1);
			matrix_r[j][i] = matrix_r[i][j];
		}
	}
	return 0;
}