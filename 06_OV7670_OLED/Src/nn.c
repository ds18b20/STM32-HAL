/******************************************************************************
nn in C
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "nn.h"

/******************************************************************************
Private Functions
*******************************************************************************/
/** slice on all channels **/
void Slice(Feature_TypeDef Src, Feature_TypeDef Dst, unsigned int start_row, unsigned int start_col)
{
    assert(start_row>=0 && start_row<=(Src.row_size-Dst.row_size));
    assert(start_col>=0 && start_col<=(Src.col_size-Dst.col_size));
	unsigned int i,j,k;
	for(i=0;i<Dst.chn_size;i++)
	{
		for(j=0;j<Dst.row_size;j++)
		{
			for(k=0;k<Dst.col_size;k++)
			{
				*(Dst.pArray+i*Dst.row_size*Dst.col_size+j*Dst.row_size+k) = *(Src.pArray+i*Src.row_size*Src.col_size+(start_row+j)*(Src.row_size)+start_col+k);
			}
		}
	}
}

/** slice on one single channel **/
void Slice_Single_Chn(Feature_TypeDef Src, Feature_TypeDef Dst, unsigned int slice_chn, unsigned int start_row, unsigned int start_col)
{
    assert(slice_chn>=0 && slice_chn<Src.chn_size);
    assert(start_row>=0 && start_row<=(Src.row_size-Dst.row_size));
    assert(start_col>=0 && start_col<=(Src.col_size-Dst.col_size));
	unsigned int j,k;
// 	for(i=0;i<Dst.chn_size;i++)
// 	{
		for(j=0;j<Dst.row_size;j++)
		{
			for(k=0;k<Dst.col_size;k++)
			{
				*(Dst.pArray+j*Dst.row_size+k) = *(Src.pArray+slice_chn*Src.row_size*Src.col_size+(start_row+j)*(Src.row_size)+start_col+k);
			}
		}
// 	}
}

/** Dot calculation on one dimension **/
float Dot(Feature_TypeDef A, Feature_TypeDef B)
{
    unsigned int size_A = A.chn_size*A.row_size*A.col_size;
    unsigned int size_B = B.chn_size*B.row_size*B.col_size;
    unsigned int i;
    float sum=0;
    assert(size_A == size_B);
    for(i=0;i<size_A;i++)
    {
        sum = sum + *(A.pArray+i)*(*(B.pArray+i));
    }
    return sum;
}

/** average on one single channel **/
float Average(Feature_TypeDef f)
{
    assert(f.chn_size == 1);
    unsigned int i;
    float sum = 0;
	for(i=0;i<f.chn_size*f.row_size*f.col_size;i++)
	{
	    sum = sum + *(f.pArray+i);
	}
    return sum / (f.chn_size*f.row_size*f.col_size);  // bug fixed (1.0/2*2=1.0   1.0/(2*2)=0.25)
}

/** max on one single channel **/
float Max(Feature_TypeDef f)
{
    assert(f.chn_size == 1);
    unsigned int i;
    float max = *(f.pArray);
	for(i=0;i<f.chn_size*f.row_size*f.col_size;i++)
	{
	    if(max<*(f.pArray+i))
	    {
	        max = *(f.pArray+i);
	    }
	}
    return max;  // bug fixed (1.0/2*2=1.0   1.0/(2*2)=0.25)
}

/** Softmax **/
void Softmax(Feature_TypeDef f)
{
    unsigned int i,j,k;
    float max, sum;
    float *pChn_base, *pRow_base, *pElement;

	for(i=0;i<f.chn_size;i++)
	{
	    pChn_base = f.pArray+i*f.row_size*f.col_size;
		for(j=0;j<f.row_size;j++)
		{
		    pRow_base = pChn_base+j*f.col_size;
		    max = *(pRow_base);
		    sum = 0;
			for(k=0;k<f.col_size;k++)
            {
                pElement = pRow_base+k;
                if(max<*pElement)
                {
                    max = *(pElement);
                }
            }
            for(k=0;k<f.col_size;k++)
            {
                pElement = pRow_base+k;
                *(pElement) = exp(*(pElement)-max);
                sum = sum+*(pElement);
            }
            for(k=0;k<f.col_size;k++)
            {
                pElement = pRow_base+k;
                *(pElement) = *(pElement)/sum;
            }
		}
	}
}

/** argmax **/
unsigned int Argmax(Feature_TypeDef f)
{
    assert(f.chn_size == 1);
    assert(f.row_size == 1);
    unsigned int i, ret_index;
    float tmp = *(f.pArray);
    for(i=0;i<f.col_size;i++)
    {
        if(tmp<*(f.pArray+i))
        {
            tmp = *(f.pArray+i);
            ret_index = i;
        }
    }
    return ret_index;
}

/** Function: Generate a Feature **/
Feature_TypeDef Generate_Feature(char name[], unsigned int channel_size, unsigned int row_size, unsigned int column_size)
{
	float* pFeature = (float *)calloc(channel_size*row_size*column_size, sizeof(float));
	if(NULL == pFeature)
    {
        printf("Memory calloc failed!\n");
        exit(-1);
    }
    Feature_TypeDef feature;
    strcpy(feature.name, name);
//    feature.name = name;
    feature.chn_size = channel_size;
    feature.row_size = row_size;
    feature.col_size = column_size;
    feature.pArray = pFeature;

    return feature;
}

/** Function: Release Feature memory **/
void Release_Memory(Feature_TypeDef *pFeature)  // must use pointer to modify values inside a struct to make modification effective!
{
	if (pFeature->pArray != NULL)
    {
        free(pFeature->pArray);  // release memory
        pFeature->pArray = NULL;  // redirect pointer to NULL
    }
}

/** print out a feature **/
void Print_Feature(Feature_TypeDef print_f)
{
    unsigned int i,j,k;
    printf("Feature name: %s\n", print_f.name);
    for(i=0;i<print_f.chn_size;i++)
    {
        printf("Chn[%d]:\n", i);
        for(j=0;j<print_f.row_size;j++)
        {
            printf("Row[%d]:", j);
            for(k=0;k<print_f.col_size;k++)
            {
                printf("%.3f, ", *(print_f.pArray+i*print_f.row_size*print_f.col_size+j*(print_f.row_size)+k));
            }
            printf("\n");
        }
        printf("\n");
    }
}

/** run Conv layer **/
void Run_Conv(Conv_Layer_TypeDef conv_layer)
{
    unsigned int i,j,k;
    for(i=0;i<conv_layer.output_f.chn_size;i++)
	{
	    for(j=0;j<conv_layer.output_f.row_size;j++)
	    {
	        for(k=0;k<conv_layer.output_f.col_size;k++)
	        {
	            Slice(conv_layer.input_f, conv_layer.sub_input_f, j*conv_layer.stride_size, k*conv_layer.stride_size);
	            *(conv_layer.output_f.pArray+i*conv_layer.output_f.row_size*conv_layer.output_f.col_size+j*conv_layer.output_f.row_size+k) = Dot(*(conv_layer.pFilters+i), conv_layer.sub_input_f)+*(conv_layer.bias_f.pArray+i);
	            // ref: http://www.runoob.com/w3cnote/c-structures-intro.html
	            // http://www.runoob.com/cprogramming/c-return-arrays-from-function.html
	        }
	    }
	}
}

/** run Relu layer **/
void Run_Relu(Relu_Layer_TypeDef relu_layer)
{
    unsigned int i;
    for(i=0;i<relu_layer.output_f.chn_size*relu_layer.output_f.row_size*relu_layer.output_f.col_size;i++)
	{
	    if(*(relu_layer.output_f.pArray+i) < 0)
        {
            *(relu_layer.output_f.pArray+i) = 0;
        }
	}
}

/** run pooling layer **/
void Run_Pooling(Pooling_Layer_TypeDef pooling_layer)
{
    unsigned int i,j,k;
    for(i=0;i<pooling_layer.output_f.chn_size;i++)
	{
	    for(j=0;j<pooling_layer.output_f.row_size;j++)
	    {
	        for(k=0;k<pooling_layer.output_f.col_size;k++)
	        {
	            Slice_Single_Chn(pooling_layer.input_f, pooling_layer.sub_input_f, i, j*pooling_layer.stride_size, k*pooling_layer.stride_size);
	            *(pooling_layer.output_f.pArray+i*pooling_layer.output_f.row_size*pooling_layer.output_f.col_size+j*pooling_layer.output_f.row_size+k) = Max(pooling_layer.sub_input_f);
	        }
	    }
	}
}

/** run affine layer **/
void Run_Affine(Affine_Layer_TypeDef affine_layer)
{
    unsigned int i;
    for(i=0;i<affine_layer.output_f.col_size;i++)
	{
        *(affine_layer.output_f.pArray+i) = Dot(*(affine_layer.pWeights+i), affine_layer.input_f)+*(affine_layer.bias_f.pArray+i);
	}
}

/** simply init a feature with integers [0,1,2...] **/
void Simple_Feature_Init(Feature_TypeDef f)
{
	unsigned int i;
	for(i=0;i<f.chn_size*f.row_size*f.col_size;i++)
	{
	    *(f.pArray+i) = i;
	}
}

/** init a feature with an array **/
void Init_Feature_by_Array(Feature_TypeDef f, const float *pInput_array)
{
    unsigned int i;
	for(i=0;i<f.chn_size*f.row_size*f.col_size;i++)
	{
	    *(f.pArray+i) = *(pInput_array+i);
	}
}
/******************************************************************************
main()
*******************************************************************************/
int main()
{
    unsigned int i;
    /************
    Convolution 1
    ************/
    /* define input feature */
    Feature_TypeDef conv1_input_feature = Generate_Feature("conv_1_input", CONV1_INPUT_CHN_SIZE, CONV1_INPUT_ROW_SIZE, CONV1_INPUT_COL_SIZE);
    Init_Feature_by_Array(conv1_input_feature, sample_image);
    /* define subset of input feature*/
    Feature_TypeDef sub_conv1_input_feature = Generate_Feature("sub_conv_1_input", CONV1_INPUT_CHN_SIZE, CONV1_FILTER_SIZE, CONV1_FILTER_SIZE);
    /* define filters */
    Feature_TypeDef conv1_filters[CONV1_FILTER_NUM];
    for(i=0;i<CONV1_FILTER_NUM;i++)
    {
        conv1_filters[i] = Generate_Feature("conv_1_filter", CONV1_INPUT_CHN_SIZE, CONV1_FILTER_SIZE, CONV1_FILTER_SIZE);
        Init_Feature_by_Array(conv1_filters[i], filter_w[i]);
    }
    /* define bias */
    Feature_TypeDef conv1_bias = Generate_Feature("conv_1_bias", CONV1_FILTER_NUM, 1, 1);
    Init_Feature_by_Array(conv1_bias, filter_b);
    /* define output feature */
    Feature_TypeDef conv1_output_feature = Generate_Feature("conv_1_output", CONV1_OUTPUT_CHN_SIZE, CONV1_OUTPUT_ROW_SIZE, CONV1_OUTPUT_COL_SIZE);
    /* layer params + feature */
    Conv_Layer_TypeDef conv1_layer;
    conv1_layer.input_f = conv1_input_feature;
    conv1_layer.sub_input_f = sub_conv1_input_feature;
    conv1_layer.bias_f = conv1_bias;
    conv1_layer.pFilters = conv1_filters;
    conv1_layer.padding_size = CONV1_PADDING_SIZE;
    conv1_layer.stride_size = CONV1_STRIDE_SIZE;
    conv1_layer.output_f = conv1_output_feature;
    /** RUN **/
    Run_Conv(conv1_layer);
    /** !!! MEMORY RELEASE !!! **/
    Release_Memory(&conv1_input_feature);
    Release_Memory(&sub_conv1_input_feature);
    Release_Memory(&conv1_bias);
    for(i=0;i<CONV1_FILTER_NUM;i++)
    {
        Release_Memory(&conv1_filters[i]);
    }

    /************
    Relu 1
    ************/
    /* define relu1 output feature */
    // new feature not needed
    Relu_Layer_TypeDef relu1_layer;
    relu1_layer.input_f = conv1_layer.output_f;  strcpy(relu1_layer.input_f.name, "relu_1_input");
    relu1_layer.output_f = conv1_layer.output_f; strcpy(relu1_layer.output_f.name, "relu_1_output");
    /** RUN **/
    Run_Relu(relu1_layer);

    /************
    Pooling1
    ************/
    /* define subset of pooling1 input feature */
    Feature_TypeDef sub_pooling1_input_feature = Generate_Feature("sub_pooling_1_input", 1, POOLING1_WINDOW_SIZE, POOLING1_WINDOW_SIZE);
    /* pooling output feature */
    Feature_TypeDef pooling1_output_feature = Generate_Feature("pooling_1_output", POOLING1_OUTPUT_CHN_SIZE, POOLING1_OUTPUT_ROW_SIZE, POOLING1_OUTPUT_COL_SIZE);
    Pooling_Layer_TypeDef pooling1_layer;
    pooling1_layer.input_f = relu1_layer.output_f;
    pooling1_layer.sub_input_f = sub_pooling1_input_feature;
    pooling1_layer.window_size = POOLING1_WINDOW_SIZE;
    pooling1_layer.stride_size = POOLING1_STRIDE_SIZE;
    pooling1_layer.output_f = pooling1_output_feature;
    /** RUN **/
    Run_Pooling(pooling1_layer);
    /** !!! MEMORY RELEASE !!! **/
    Release_Memory(&conv1_output_feature);
    Release_Memory(&sub_pooling1_input_feature);

    /************
    Affine 1
    ************/
    /* define affine1 bias */
    Feature_TypeDef affine1_bias = Generate_Feature("affine_1_bias", AFFINE1_OUTPUT_SIZE, 1, 1);
    Init_Feature_by_Array(affine1_bias, affine1_b);
    /* define Affine1 output feature */
    Feature_TypeDef affine1_output_feature = Generate_Feature("affine_1_output", 1, 1, AFFINE1_OUTPUT_SIZE);
    /* define Affine1 weights */
    Feature_TypeDef affine1_weights[AFFINE1_WEIGHT_NUM];
    for(i=0;i<AFFINE1_WEIGHT_NUM;i++)
    {
        affine1_weights[i] = Generate_Feature("affine_1_weight", 1, 1, AFFINE1_WEIGHT_SIZE);
        Init_Feature_by_Array(affine1_weights[i], affine1_w[i]);
    }
    Affine_Layer_TypeDef affine1_layer;
    affine1_layer.input_f = pooling1_layer.output_f;
    affine1_layer.bias_f = affine1_bias;
    affine1_layer.pWeights = affine1_weights;
    affine1_layer.input_size = pooling1_layer.output_f.chn_size*pooling1_layer.output_f.row_size*pooling1_layer.output_f.col_size;
    affine1_layer.output_size = AFFINE1_OUTPUT_SIZE;
    affine1_layer.output_f = affine1_output_feature;
    /** RUN **/
    Run_Affine(affine1_layer);
    /** !!! MEMORY RELEASE !!! **/
    Release_Memory(&pooling1_output_feature);
    Release_Memory(&affine1_bias);
    for(i=0;i<AFFINE1_WEIGHT_NUM;i++)
    {
        Release_Memory(&affine1_weights[i]);
    }

    /************
    Relu 2
    ************/
    /* define relu1 output feature */
    // new feature not needed
    Relu_Layer_TypeDef relu2_layer;
    relu2_layer.input_f = affine1_layer.output_f;  strcpy(relu2_layer.input_f.name, "relu_2_input");
    relu2_layer.output_f = affine1_layer.output_f; strcpy(relu2_layer.output_f.name, "relu_2_output");
    /** RUN **/
    Run_Relu(relu2_layer);

    /************
    Affine 2
    ************/
    /* define affine1 bias */
    Feature_TypeDef affine2_bias = Generate_Feature("affine_2_bias", AFFINE2_OUTPUT_SIZE, 1, 1);
    Init_Feature_by_Array(affine2_bias, affine2_b);
    /* define Affine2 output feature */
    Feature_TypeDef affine2_output_feature = Generate_Feature("affine_2_output", 1, 1, AFFINE2_OUTPUT_SIZE);
    /* define Affine2 weights */
    Feature_TypeDef affine2_weights[AFFINE2_WEIGHT_NUM];
    for(i=0;i<AFFINE2_WEIGHT_NUM;i++)
    {
        affine2_weights[i] = Generate_Feature("affine_2_weight", 1, 1, AFFINE2_WEIGHT_SIZE);
        Init_Feature_by_Array(affine2_weights[i], affine2_w[i]);
    }
    Affine_Layer_TypeDef affine2_layer;
    affine2_layer.input_f = relu2_layer.output_f;
    affine2_layer.bias_f = affine2_bias;
    affine2_layer.pWeights = affine2_weights;
    affine2_layer.input_size = AFFINE1_OUTPUT_SIZE;
    affine2_layer.output_size = AFFINE2_OUTPUT_SIZE;
    affine2_layer.output_f = affine2_output_feature;
    /** RUN **/
    Run_Affine(affine2_layer);
    /** !!! MEMORY RELEASE !!! **/
    Release_Memory(&affine1_output_feature);
    Release_Memory(&affine2_bias);
    for(i=0;i<AFFINE2_WEIGHT_NUM;i++)
    {
        Release_Memory(&affine2_weights[i]);
    }

    /************
    Predict
    ************/
    Print_Feature(affine2_layer.output_f);
    printf("Prediction: %d\n", Argmax(affine2_layer.output_f));

    /************
    Softmax
    ************/
//    Softmax(affine2_layer.output_f);
//    Print_Feature(affine2_layer.output_f);

    Release_Memory(&affine2_output_feature);
    return 0;
}

// printf("%p\n", conv1_input_feature.pArray);
