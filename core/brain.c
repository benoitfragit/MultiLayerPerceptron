#include "brain.h"
#include <string.h>

double
backpropagate_output_layer(Network* network, const int number_of_output, const double* desired)
{
    double error = 0.0;

    if (network && desired)
    {
        //backpropagate the error to the output layer
        Layer* output_layer = layer(network, network->_number_of_layer - 1);

        if (output_layer != NULL && output_layer->_number_of_neuron == number_of_output)
        {
            int neuron_index = 0;

            for (neuron_index = 0; neuron_index < output_layer->_number_of_neuron; ++neuron_index)
            {
                Neuron* oNeuron = neuron(output_layer, neuron_index);

                if (oNeuron != NULL)
                {
                    append_delta(oNeuron, oNeuron->_out - desired[neuron_index]);

                    error += ((desired[neuron_index] - oNeuron->_out) * (desired[neuron_index] - oNeuron->_out)) / 2.0;
                }
            }
        }
    }

    return error;
}

void
backpropagate_hidden_layer(Network* network, const int layer_index)
{
    int j;

    if (network && 0 <= layer_index && layer_index < network->_number_of_layer)
    {
        Layer* pLayer = layer(network, layer_index);

        if (pLayer != NULL)
        {
            for (j = 0; j < pLayer->_number_of_neuron; ++j)
            {
                Neuron *pNeuron = pLayer->_neurons[j];

                if (pNeuron)
                {
                    const Synapse* neural_synapse = synapse(network, pLayer->_id, pNeuron->_id);

                    if (neural_synapse)
                    {
                        Layer* nLayer = layer(network, neural_synapse->_output_layer);

                        if (nLayer)
                        {
                            Neuron* nNeuron = neuron(nLayer, neural_synapse->_output_neuron);

                            if (nNeuron)
                            {
                                append_delta(pNeuron, get_weighted_delta(nNeuron, neural_synapse->_input_index));
                            }
                        }
                    }
                }
            }
        }
    }
}

void
feedforward(Network *network)
{
    int synapse_index = 0, i;
    Synapse* synapse = NULL;
    Layer *input_layer = NULL, *output_layer = NULL;
    Neuron *input_neuron = NULL, *output_neuron = NULL;

    if (network != NULL)
    {
        for (synapse_index = 0; synapse_index < network->_number_of_synapse; ++synapse_index)
        {
            synapse      = network->_synapses[synapse_index];
            input_layer  = layer(network, synapse->_input_layer);
            output_layer = layer(network, synapse->_output_layer);

            if (input_layer && output_layer)
            {
                input_neuron  = neuron(input_layer, synapse->_input_neuron);
                output_neuron = neuron(output_layer, synapse->_output_neuron);

                if (input_neuron && output_neuron)
                {
                    propagate(output_neuron, input_neuron->_out, synapse->_input_index);
                    activate(output_neuron);
                }
            }
        }

        output_layer = layer(network, network->_number_of_layer - 1);
        if (output_layer)
        {
            for (i = 0; i < output_layer->_number_of_neuron; ++i)
            {
                output_neuron = neuron(output_layer, i);

                if (output_neuron)
                {
                    network->_output[i] = output_neuron->_out;
                }
            }
        }
    }
}

double
backpropagate(Network *network, const int number_of_output, const double *desired)
{
    int i, j;
    const double error = backpropagate_output_layer(network, number_of_output, desired);

    for (i = network->_number_of_layer - 2; i >= 0; --i)
    {
        backpropagate_hidden_layer(network, i);
    }

    // now update all weight
    for (i = network->_number_of_layer - 1; i >= 0; --i)
    {
        Layer* pLayer = layer(network, i);

        if (pLayer != NULL)
        {
            for (j = 0; j < pLayer->_number_of_neuron; ++j)
            {
                Neuron *pNeuron = pLayer->_neurons[j];

                if (pNeuron)
                {
                    update(pNeuron);
                }
            }
        }
    }

    BRAIN_INFO("Brain quadratic error id %lf", error);

    return error;
}

void
train(Network *network, const Data* data, const double target_error, const int max_iter)
{
	int iteration = 0, subset_index = 0, index = 0;
	double error = target_error + 1.0;

	if (network && data && target_error >= 0 && max_iter >= 1)
	{
		do 
		{
			for (subset_index = 0; subset_index < data->_subset_length; ++subset_index)
			{
				index = data->_subset[subset_index];
				
				set_network_input(network, data->_signal_length, data->_signals[index]);
				feedforward(network);
				error = backpropagate(network, data->_observation_length, data->_observations[index]);

				if (target_error <= error)
				{
					BRAIN_INFO("Network has beene successfully trained");
					network->_is_trained = 1;
					return;
				}
			}
			
			++iteration;
		} while ((iteration < max_iter) && (target_error > error));
	}
	
	if (error > target_error)
	{
		BRAIN_CRITICAL("Unable to train the neural network, target error = %lf, error = %lf", target_error, error);
		network->_is_trained = 0;
		return;
	}

	network->_is_trained = 1;
}
