#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dsverifier/initialization.h"

//gcc -o main esbmcnn.c
#define arraySize(x) (int)(sizeof(x) / sizeof((x)[0]))

float *neuronsSimbolicRestrictions;
int restrictionNeuronsWidth;

typedef struct
{
  int neurons;
  float *weights;
  float *bias;
  float *restrictionsHIGHER;
  float *restrictionsLOWER;
  float *concretization;
  unsigned short *allowsSimbolic;
} layer;

typedef layer *esbmc_layer;

typedef struct
{
  esbmc_layer layer;
  esbmc_layer prev;
  esbmc_layer next;
} layers;

typedef layers *esbmc_layers;

typedef struct nnet
{
  float *inputs;
  float *outputs;
  unsigned short isPatternNet;
  layer layers[0];
  int layersInstanciatedIndex;
  int layersNumber;
} esbmc_nnet;

void printfVector(float *pointer, int size)
{
  for (int i = 0; i < size; i++)
  {
    printf("%.6f \n", pointer[i]);
  }
}

void printIVector(int *pointer, int size)
{
  for (int i = 0; i < size; i++)
  {
    printf("%d \n", pointer[i]);
  }
}

void printfLayers(esbmc_nnet *net)
{
  for (int i = 0; i < net->layersNumber; i++)
  {
    printf("Layer: %d Neurons: %d \n", i, net->layers[i].neurons);
  }
}

void printfMatrix(float *mat, int rows, int column)
{
  int i = 0, j = 0;
  for (i = 0; i < rows; i++)
  {
    for (j = 0; j < column; j++)
    {
      printf("  %.6f", mat[i * column + j]);
    }
    printf("\n");
  }
}

void printNeuralNetworkDescriptors(esbmc_nnet *net)
{
  //printf("#include <stdio.h> \n#include <math.h>\n#include <stdlib.h>\n#include <time.h>\nfloat UpLinearRelaxation(float input, float up, float low) {\n  float relaxation = (up/(up-low))*(input-low);\n  return relaxation;\n}\nfloat LowLinearRelaxation(float input, float up, float low) {\n  float relaxation = up/(up-low)*(input);\n  return relaxation;\n}\nint main(){\n    clock_t t;\n    t = clock();\n    float y = nondet_float();\n    __ESBMC_assume(y >= 0 && y <= 1);\n    float x = nondet_float();\n    __ESBMC_assume(x >= 0 && x <= 1);\n    float safeLimit = 3.8;\n");
  for (int n = 1; n < net->layersNumber; n++)
  {
    printf("W[%d] = {", n);
    int rows = net->layers[n].neurons;
    int columns = net->layers[n - 1].neurons;
    for (int j = 0; j < rows; j++)
    {
      for (int i = 0; i < columns; i++)
      {
        printf("  %.6f,", net->layers[n].weights[i + j * columns]);
      }
      printf("\n");
    }
    printf("Bias[%d] = {", n);
    for (int j = 0; j < rows; j++)
    {
      printf("  %.6f,", net->layers[n].bias[j]);
    }
    printf("}\n");
  }

  //__ESBMC_assume(-3*x + 2*y > 0);\n    __ESBMC_assume(4*x + 1*y > 0);\n    __ESBMC_assert(x + 3*y - safeLimit < 0, \"Unsafe\");\n}");
}

esbmc_layer createLayer(int neurons, int inputs, float *weights, float *bias)
{
  float *layerWeights;
  float *layerBias;
  layerWeights = (float *)malloc(sizeof(float) * inputs * neurons);
  layerBias = (float *)malloc(sizeof(float) * inputs * neurons);
  esbmc_layer layer = (esbmc_layer)malloc(sizeof(layer));
  layer->neurons = neurons;
  layer->weights = layerWeights;
  layer->bias = layerBias;
  return layer;
}

esbmc_nnet *initializeNN(int *layersDescription, int layersNumber)
{
  esbmc_nnet *nnet = (esbmc_nnet *)malloc(sizeof(esbmc_nnet) + (layersNumber + 2) * sizeof(layer));
  nnet->layersNumber = layersNumber + 2;
  for (int i = 1; i <= layersNumber; i++)
  {
    nnet->layers[i].neurons = layersDescription[i - 1];
  }
  return nnet;
}

void configNet(esbmc_nnet *net, int inputs, int outputs)
{
  net->layers[0].neurons = inputs;
  net->layers[net->layersNumber - 1].neurons = outputs;
  net->isPatternNet = 0;
  restrictionNeuronsWidth = inputs + 1;
  for (int i = 1; i < net->layersNumber; i++)
  {
    int neurons = net->layers[i].neurons;
    int previous = net->layers[i - 1].neurons;
    net->layers[i].weights = (float *)malloc(sizeof(float) * neurons * previous);
    net->layers[i].bias = (float *)malloc(sizeof(float) * neurons);
    net->layers[i].restrictionsHIGHER = (float *)malloc(sizeof(float) * neurons * (inputs + 1));
    net->layers[i].restrictionsLOWER = (float *)malloc(sizeof(float) * neurons * (inputs + 1));
    net->layers[i].concretization = (float *)malloc(sizeof(float) * neurons * 2);
    net->layers[i].allowsSimbolic = (unsigned short *)malloc(sizeof(unsigned short) * neurons * 2);
  }
}

void addLayerDescription(esbmc_nnet *net, int index, float *weights, float *bias)
{
  if (index == 0)
  {
    printf("Index must be a positive integer. \n");
    exit(0);
  }
  else if (index >= net->layersNumber)
  {
    printf("Index can't be outside the neural net. Max index: %d. \n", net->layersNumber - 1);
    exit(0);
  }
  else
  {
    int inputs = net->layers[index - 1].neurons;
    int neurons = net->layers[index].neurons;
    memcpy(net->layers[index].weights, weights, sizeof(float) * inputs * neurons);
    memcpy(net->layers[index].bias, bias, sizeof(float) * neurons);
  }
}

void addLayer(esbmc_nnet *net, esbmc_layer layer)
{
  if (net->layersNumber == 0)
  {
    //    net->layers->layer = layer;
    net->layersNumber++;
  }
  else
  {
    //    net->layers->layer->neurons = layer -> neurons;
    net->layersNumber++;
  }
}

void computeLayerSimbolicPropagation(float *restrictions, int restrictionsSize, float bias, int neuronsPrevious, float *simbolicOutput)
{
  for (int i = 0; i < restrictionsSize; i++)
  {
    for (int j = 0; j < neuronsPrevious; j++)
    {
      if (j == 0)
      {
        simbolicOutput[i] = restrictions[i + (j * (restrictionsSize))];
        //printf("SAIDA %d : %.6f \n",i+(j*(restrictionsSize)), restrictions[i+(j*(restrictionsSize))]);
      }
      else
      {
        simbolicOutput[i] += restrictions[i + (j * (restrictionsSize))];
        //printf("SAIDA %d : %.6f \n",i+(j*(restrictionsSize)), restrictions[i+(j*(restrictionsSize))]);
        if (j == neuronsPrevious - 1)
        {
          //printf("SAIDA: %.6f \n",simbolicOutput[i]);
        }
      }
    }
  }
  simbolicOutput[restrictionsSize - 1] += bias;
}

void positiveWeights(float *coefficients, float *positiveWeights)
{
  for (int i = 0; i < restrictionNeuronsWidth - 1; i++)
  {
    if (coefficients[i] > 0)
      positiveWeights[i] = coefficients[i];
    else
      positiveWeights[i] = 0;
  }
}

void negativeWeights(float *coefficients, float *negativeWeights)
{
  for (int i = 0; i < restrictionNeuronsWidth - 1; i++)
  {
    if (coefficients[i] < 0)
      negativeWeights[i] = coefficients[i];
    else
      negativeWeights[i] = 0;
  }
}

void maximumVector(float *intervals, float *maximumVector)
{
  for (int i = 0; i < restrictionNeuronsWidth - 1; i++)
  {
    maximumVector[i] = intervals[(i * 2) + 1];
  }
}

void minimumVector(float *intervals, float *minimumVector)
{
  for (int i = 0; i < restrictionNeuronsWidth - 1; i++)
  {
    minimumVector[i] = intervals[i * 2];
  }
}

void concretize(float *interval, esbmc_nnet *net, int layer, int neuron)
{
  float resultUp = 0;
  float resultLow = 0;
  float *positives;
  positives = (float *)malloc(sizeof(float) * (restrictionNeuronsWidth - 1));
  float *negatives;
  negatives = (float *)malloc(sizeof(float) * (restrictionNeuronsWidth - 1));
  float *maximum;
  maximum = (float *)malloc(sizeof(float) * (restrictionNeuronsWidth - 1));
  float *minimum;
  minimum = (float *)malloc(sizeof(float) * (restrictionNeuronsWidth - 1));

  positiveWeights(&(net->layers[layer].restrictionsHIGHER[neuron * restrictionNeuronsWidth]), positives);
  negativeWeights(&(net->layers[layer].restrictionsHIGHER[neuron * restrictionNeuronsWidth]), negatives);
  maximumVector(interval, maximum);
  minimumVector(interval, minimum);
  for (int i = 0; i < restrictionNeuronsWidth - 1; i++)
  {
    resultUp += positives[i] * maximum[i] + negatives[i] * minimum[i];
    resultLow += positives[i] * minimum[i] + negatives[i] * maximum[i];
  }
  resultUp += net->layers[layer].restrictionsHIGHER[neuron * restrictionNeuronsWidth + (restrictionNeuronsWidth - 1)];
  resultLow += net->layers[layer].restrictionsLOWER[neuron * restrictionNeuronsWidth + (restrictionNeuronsWidth - 1)];
  if (resultUp <= 0)
  {
    net->layers[layer].allowsSimbolic[neuron * 2] = 0;
    net->layers[layer].allowsSimbolic[(neuron * 2) + 1] = 0;
    net->layers[layer].concretization[neuron * 2] = 0;
    net->layers[layer].concretization[(neuron * 2) + 1] = 0;
  }
  else if (resultUp > 0 && resultLow < 0)
  {
    net->layers[layer].allowsSimbolic[neuron * 2] = 0;
    net->layers[layer].allowsSimbolic[(neuron * 2) + 1] = 1;
    net->layers[layer].concretization[neuron * 2] = 0;
    net->layers[layer].concretization[(neuron * 2) + 1] = resultUp;
  }
  else
  {
    net->layers[layer].allowsSimbolic[neuron * 2] = 1;
    net->layers[layer].allowsSimbolic[(neuron * 2) + 1] = 1;
    net->layers[layer].concretization[neuron * 2] = resultLow;
    net->layers[layer].concretization[(neuron * 2) + 1] = resultUp;
  }
  free(positives);
  free(negatives);
  free(maximum);
  free(minimum);
}

void generateSimbolicBoundedEquationsFirstLayer(float *interval, esbmc_nnet *net)
{
  int inputs = net->layers[0].neurons;
  int currentNeurons = net->layers[1].neurons;
  for (int j = 0; j < currentNeurons; j++)
  {
    memcpy(&(net->layers[1].restrictionsLOWER[j * restrictionNeuronsWidth]), &(net->layers[1].weights[j * inputs]), sizeof(float) * inputs);
    memcpy(&(net->layers[1].restrictionsHIGHER[j * restrictionNeuronsWidth]), &(net->layers[1].weights[j * inputs]), sizeof(float) * inputs);
    memcpy(&(net->layers[1].restrictionsLOWER[(j * restrictionNeuronsWidth) + inputs]), &(net->layers[1].bias[j]), sizeof(float));
    memcpy(&(net->layers[1].restrictionsHIGHER[(j * restrictionNeuronsWidth) + inputs]), &(net->layers[1].bias[j]), sizeof(float));
    concretize(interval, net, 1, j);
  }
}

void generateSimbolicBoundedEquations(float *interval, int layer, esbmc_nnet *net)
{
  int currentNeurons = net->layers[layer].neurons;
  int previousNeurons = net->layers[layer - 1].neurons;
  int inputs = net->layers[0].neurons;
  float *restrictionsAuxUp;
  float *restrictionsAuxLow;
  restrictionsAuxUp = (float *)malloc(sizeof(float) * previousNeurons * (restrictionNeuronsWidth));
  restrictionsAuxLow = (float *)malloc(sizeof(float) * previousNeurons * (restrictionNeuronsWidth));
  for (int neuron = 0; neuron < currentNeurons; neuron++)
  {
    for (int previousNeuron = 0; previousNeuron < previousNeurons; previousNeuron++)
    {
      if (net->layers[layer - 1].allowsSimbolic[previousNeuron * 2] == 1)
      {
        for (int i = 0; i < restrictionNeuronsWidth; i++)
        {
          restrictionsAuxUp[previousNeuron * restrictionNeuronsWidth + i] = net->layers[layer].weights[neuron * previousNeurons] * net->layers[layer - 1].restrictionsHIGHER[previousNeuron * restrictionNeuronsWidth + i];
          restrictionsAuxLow[previousNeuron * restrictionNeuronsWidth + i] = net->layers[layer].weights[neuron * previousNeurons] * net->layers[layer - 1].restrictionsLOWER[previousNeuron * restrictionNeuronsWidth + i];
        }
      }
      else
      {
        for (int i = 0; i < restrictionNeuronsWidth - 1; i++)
        {
          restrictionsAuxUp[previousNeuron * restrictionNeuronsWidth + i] = 0;
          restrictionsAuxLow[previousNeuron * restrictionNeuronsWidth + i] = 0;
        }
        if (net->layers[layer].weights[neuron * previousNeurons] >= 0)
        {
          restrictionsAuxLow[previousNeuron * restrictionNeuronsWidth + inputs] = net->layers[layer].weights[neuron * previousNeurons] * net->layers[layer - 1].concretization[previousNeuron * 2];
          restrictionsAuxUp[previousNeuron * restrictionNeuronsWidth + inputs] = net->layers[layer].weights[neuron * previousNeurons] * net->layers[layer - 1].concretization[(previousNeuron * 2) + 1];
        }
        else
        {
          restrictionsAuxLow[previousNeuron * restrictionNeuronsWidth + inputs] = net->layers[layer].weights[neuron * previousNeurons] * net->layers[layer - 1].concretization[(previousNeuron * 2) + 1];
          restrictionsAuxUp[previousNeuron * restrictionNeuronsWidth + inputs] = net->layers[layer].weights[neuron * previousNeurons] * net->layers[layer - 1].concretization[previousNeuron * 2];
        }
      }
    }
    computeLayerSimbolicPropagation(restrictionsAuxLow, inputs + 1, net->layers[layer].bias[neuron], previousNeurons, &(net->layers[layer].restrictionsLOWER[neuron * restrictionNeuronsWidth]));
    computeLayerSimbolicPropagation(restrictionsAuxUp, inputs + 1, net->layers[layer].bias[neuron], previousNeurons, &(net->layers[layer].restrictionsHIGHER[neuron * restrictionNeuronsWidth]));
    concretize(interval, net, layer, neuron);
  }
  free(restrictionsAuxLow);
  free(restrictionsAuxUp);
}

void printConcretizations(esbmc_nnet *net)
{
  int layers = net->layersNumber;
  printf("LAYER NUMBER %d \n", layers);
  for (int i = 1; i < layers; i++)
  {
    printf("LAYER %d \n", i);
    int currentLayer = net->layers[i].neurons;
    for (int j = 0; j < currentLayer; j++)
    {
      printf("Allows simbolic: %d   %d Concretization: %.6f   %.6f \n", net->layers[i].allowsSimbolic[j * 2],
             net->layers[i].allowsSimbolic[(j * 2) + 1], net->layers[i].concretization[j * 2],
             net->layers[i].concretization[(j * 2) + 1]);
    }
    printfMatrix(&(net->layers[i].restrictionsLOWER[0]), currentLayer, restrictionNeuronsWidth);
    printfMatrix(&(net->layers[i].restrictionsHIGHER[0]), currentLayer, restrictionNeuronsWidth);
  }
}

void getSimbolicNNPropagation(esbmc_nnet *net, float *intervals)
{
  int inputs = net->layers[0].neurons;
  //printf("INPUTS %d \n",inputs);
  int layers = net->layersNumber;
  //printf("LAYERS %d \n",layers);
  //Luiz - 20200701 - restrictionNeuronsWidth represents the input size plus bias, that is the restriction elements.
  int totalNumberOfNeurons = 0;
  int neuronsSimbolicIndexVec[layers];
  neuronsSimbolicIndexVec[0] = 0;
  for (int i = 1; i < layers; i++)
  {
    totalNumberOfNeurons += net->layers[i].neurons;
    neuronsSimbolicIndexVec[i] = neuronsSimbolicIndexVec[i - 1] + net->layers[i].neurons;
  }
  //printf("TOTAL NEURONS %d \n",totalNumberOfNeurons);
  //printIVector(neuronsSimbolicIndexVec, layers);

  neuronsSimbolicRestrictions = (float *)malloc(sizeof(float) * restrictionNeuronsWidth * totalNumberOfNeurons);
  float *restrictionsAux;
  int currentLayerNeurons;
  int previousLayerNeurons;
  int layersNeuronIndex;
  int previousLayersNeuronIndex;
  for (int l = 1; l < layers; l++)
  {
    //printf("RESTRICTIONS OF LAYER1: \n");
    currentLayerNeurons = net->layers[l].neurons;
    if (l == 1)
    {
      for (int j = 0; j < currentLayerNeurons; j++)
      {
        //for(int i = 0; i < inputs; i++){
        //neuronsSimbolicRestrictions[i + j*(restrictionNeuronsWidth)] = net->layers[l].weights[i + j*inputs];
        memcpy(&(neuronsSimbolicRestrictions[j * restrictionNeuronsWidth]), &(net->layers[l].weights[j * inputs]), sizeof(float) * inputs);
        memcpy(&(neuronsSimbolicRestrictions[(j * restrictionNeuronsWidth) + inputs]), &(net->layers[l].bias[j]), sizeof(float));
        //printf("RESTRICTION %d : %.6f \n",i + j*(inputs+1), neuronsSimbolicRestrictions[i + j*(inputs+1)]);
        //}
        //neuronsSimbolicRestrictions[j*(restrictionNeuronsWidth) + inputs] = net->layers[l].bias[j];
        //printf("RESTRICTION %d : %.6f \n",j*(inputs+1) + inputs, neuronsSimbolicRestrictions[j*(inputs+1) + inputs]);
      }
      generateSimbolicBoundedEquationsFirstLayer(intervals, net);
    }
    else
    {
      //printf("RESTRICTIONS OF LAYER%d: \n", l);
      currentLayerNeurons = net->layers[l].neurons;
      previousLayerNeurons = net->layers[l - 1].neurons;
      layersNeuronIndex = neuronsSimbolicIndexVec[l - 1] * (restrictionNeuronsWidth);
      previousLayersNeuronIndex = neuronsSimbolicIndexVec[l - 2] * (restrictionNeuronsWidth);
      //printf("previousLayersNeuronIndex %d: \n", previousLayersNeuronIndex);
      restrictionsAux = (float *)malloc(sizeof(float) * previousLayerNeurons * (restrictionNeuronsWidth));
      for (int j = 0; j < currentLayerNeurons; j++)
      {
        for (int k = 0; k < previousLayerNeurons; k++)
        {
          for (int i = 0; i <= inputs; i++)
          {
            restrictionsAux[i + k * (restrictionNeuronsWidth)] = net->layers[l].weights[k + j * previousLayerNeurons] * neuronsSimbolicRestrictions[previousLayersNeuronIndex + (i + k * (restrictionNeuronsWidth))];
            //printf("RESTRICTION %d : %.6f \n",i + j*(inputs+1), restrictionsAux[i + j*(inputs+1)]);
          }
        }
        //printfMatrix(restrictionsAux, 2, 3);
        computeLayerSimbolicPropagation(restrictionsAux, inputs + 1, net->layers[l].bias[j], previousLayerNeurons, &neuronsSimbolicRestrictions[layersNeuronIndex + j * (inputs + 1)]);
      }
      generateSimbolicBoundedEquations(intervals, l, net);
    }
  }
  //printfMatrix(neuronsSimbolicRestrictions, 3, 3);
}

void printIntervals(float *inputsInterval)
{
  int inputsSize = restrictionNeuronsWidth - 1;
  for (int i = 0; i < inputsSize; i++)
  {
    printf("  float x%d = nondet_float();\n", i);
    printf("  __ESBMC_assume(x%d >= %.6f && x%d <= %.6f);\n", i, inputsInterval[i * 2], i, inputsInterval[(i * 2) + 1]);
  }
}

void printRestrictions(esbmc_nnet *net)
{
  int inputs = net->layers[0].neurons;
  int layers = net->layersNumber;
  int neuronsSimbolicIndexVec[layers];
  neuronsSimbolicIndexVec[0] = 0;
  if (layers <= 2)
    return;
  for (int i = 1; i < layers; i++)
  {
    neuronsSimbolicIndexVec[i] = neuronsSimbolicIndexVec[i - 1] + net->layers[i].neurons;
  }
  int lastNeuronIndex = neuronsSimbolicIndexVec[layers - 2];
  for (int i = 0; i < lastNeuronIndex; i++)
  {
    printf("  __ESBMC_assume(");
    for (int j = 0; j < restrictionNeuronsWidth; j++)
    {
      if (j == 0)
      {
        printf("%.6f*x%d ", neuronsSimbolicRestrictions[i * restrictionNeuronsWidth + j], j);
      }
      else if (j < restrictionNeuronsWidth - 1)
      {
        if (neuronsSimbolicRestrictions[i * restrictionNeuronsWidth + j] < 0)
        {
          printf("- %.6f*x%d ", (-1) * neuronsSimbolicRestrictions[i * restrictionNeuronsWidth + j], j);
        }
        else if (neuronsSimbolicRestrictions[i * restrictionNeuronsWidth + j] > 0)
        {
          printf("+ %.6f*x%d ", neuronsSimbolicRestrictions[i * restrictionNeuronsWidth + j], j);
        }
      }
      else
      {
        if (neuronsSimbolicRestrictions[i * restrictionNeuronsWidth + j] < 0)
        {
          printf("- %.6f > 0);\n", (-1) * neuronsSimbolicRestrictions[i * restrictionNeuronsWidth + j]);
        }
        else if (neuronsSimbolicRestrictions[i * restrictionNeuronsWidth + j] > 0)
        {
          printf("+ %.6f > 0);\n", neuronsSimbolicRestrictions[i * restrictionNeuronsWidth + j]);
        }
        else
        {
          printf("> 0);\n");
        }
      }
    }
  }
}

void printProperties(esbmc_nnet *net, float safeLimit)
{
  int layers = net->layersNumber;
  int neuronsSimbolicIndexVec[layers];
  neuronsSimbolicIndexVec[0] = 0;
  for (int i = 1; i < layers; i++)
  {
    neuronsSimbolicIndexVec[i] = neuronsSimbolicIndexVec[i - 1] + net->layers[i].neurons;
  }
  int startIndex;
  int lastNeuronIndex;
  if (net->layersNumber < 2)
  {
    startIndex = 0;
    lastNeuronIndex = neuronsSimbolicIndexVec[layers - 1];
  }
  else
  {
    startIndex = neuronsSimbolicIndexVec[layers - 2];
    lastNeuronIndex = neuronsSimbolicIndexVec[layers - 1];
  }
  printf("  __ESBMC_assert(");
  for (int i = startIndex; i < lastNeuronIndex; i++)
  {
    for (int j = 0; j < restrictionNeuronsWidth; j++)
    {
      if (j == 0)
      {
        printf("%.6f*x%d ", neuronsSimbolicRestrictions[i * restrictionNeuronsWidth + j], j);
      }
      else if (j < restrictionNeuronsWidth - 1)
      {
        if (neuronsSimbolicRestrictions[i * restrictionNeuronsWidth + j] < 0)
        {
          printf("- %.6f*x%d ", (-1) * neuronsSimbolicRestrictions[i * restrictionNeuronsWidth + j], j);
        }
        else if (neuronsSimbolicRestrictions[i * restrictionNeuronsWidth + j] > 0)
        {
          printf("+ %.6f*x%d ", neuronsSimbolicRestrictions[i * restrictionNeuronsWidth + j], j);
        }
      }
      else
      {
        if (neuronsSimbolicRestrictions[i * restrictionNeuronsWidth + j] < 0)
        {
          printf("- %.6f <= %.6f, \"Safety property violated\");\n", (-1) * neuronsSimbolicRestrictions[i * restrictionNeuronsWidth + j], safeLimit);
        }
        else if (neuronsSimbolicRestrictions[i * restrictionNeuronsWidth + j] > 0)
        {
          printf("+ %.6f <= %.6f, \"Safety property violated\");\n", neuronsSimbolicRestrictions[i * restrictionNeuronsWidth + j], safeLimit);
        }
        else
        {
          printf("<= %.6f, \"Safety property violated\");\n", safeLimit);
        }
      }
    }
  }
}

void printSimbolicPropagationCode(esbmc_nnet *net, float *inputsInterval)
{
  printf("#include <stdio.h>\n#include <math.h>\n#include <stdlib.h>\n#include <time.h>\n  float UpLinearRelaxation(float input, float up, float low) {\n    float relaxation = (up/(up-low))*(input-low);\n    return relaxation;\n  }\n\n  float LowLinearRelaxation(float input, float up, float low) {\n    float relaxation = up/(up-low)*(input);\n    return relaxation;\n  }\n\n");
  printf("int main(){\n");
  //printing intervals;
  printIntervals(inputsInterval);
  printf("//RESTRICTIONS \n");
  printRestrictions(net);
  printf("//PROPERTIES \n");
  printProperties(net, 5.5);
  printf("}\n");
}

int main()
{
  int hiddenLayers[1] = {2};
  // float w1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  // float bias1[10] = {1, 2, 3, 4, 5, 7, 7, 8, 9, 10};
  // float w2[20] = {1, 2, 3, 4, 5, 7, 7, 8, 9, 10, 1, 2, 3, 4, 5, 7, 7, 8, 9, 10};
  // float bias2[2] = {2, 20};
  // float w3[8] = {31, 32, 33, 34, 35, 37, 37, 38};
  // float bias3[4] = {2, 20, 4, 5};
  // float w4[40] = {1, 2, 3, 4,
  //   5, 7, 7, 8,
  //   9, 10, 1, 2,
  //   3, 4, 5, 7,
  //   7, 8, 9, 10,
  //   1, 2, 3, 4,
  //   5, 7, 7, 8,
  //   9, 10, 1, 2,
  //   3, 4, 5, 7,
  //   7, 8, 9, 10};
  // float bias4[10] = {1, 2, 3, 4, 5, 7, 7, 8, 9, 10};
  float inputIntervals[4] = {0, 1,
                             0, 1};

  float w1[4] = {-3, 2,
                 4, 1};

  float bias1[2] = {0, 0};

  float w2[2] = {1, 1};

  float bias2[1] = {0};

  esbmc_nnet *nnet = initializeNN(hiddenLayers, arraySize(hiddenLayers));
  configNet(nnet, 2, 1);
  addLayerDescription(nnet, 1, w1, bias1);
  addLayerDescription(nnet, 2, w2, bias2);
  // addLayerDescription(nnet, 3, w3, bias3);
  // addLayerDescription(nnet, 4, w4, bias4);
  //printfLayers(nnet);
  //printNeuralNetworkDescriptors(nnet);
  getSimbolicNNPropagation(nnet, inputIntervals);

  //printSimbolicPropagationCode(nnet, inputIntervals);
  //printConcretizations(nnet);
}