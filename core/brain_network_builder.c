#include "brain_network_builder.h"

struct Network
{
    Layer_t   *_layers;
    Synapse_t *_synapses;
    double    *_output;

	int _is_trained;

    int _number_of_synapse;
    int _number_of_layer;
} Network;

void
set_trained(Network_t network, const int trained)
{
	if (network)
	{
		network->_is_trained = trained;
	}
}

int
is_trained(const Network_t network)
{
	if (network)
	{
		return network->_is_trained;
	}

	return 0;
}

void
set_output(Network_t network, const int index, const double value)
{
	if (network
	&&  (0 <= index)
	&&  (index < network->_number_of_layer))
	{
		network->_output[index] = value;
	}
}

const double*
getoutput(const Network_t network)
{
    if (network != NULL)
    {
        return network->_output;
    }

    return NULL;
}

Layer_t
layer(Network_t network, const int layer_index)
{
    if (network != NULL
    && network->_layers != NULL
    && 0 <= layer_index
    && layer_index < network->_number_of_layer)
    {
        return network->_layers[layer_index];
    }

    return NULL;
}

Synapse_t
synapse(Network_t network, const int layer_id, const int neuron_id)
{
    int i;

    if (network != NULL)
    {
        for (i = 0; i < network->_number_of_synapse; ++i)
        {
            if (get_input_layer(network->_synapses[i]) == layer_id && get_input_neuron(network->_synapses[i]) == neuron_id)
            {
                return network->_synapses[i];
            }
        }
    }

    return NULL;
}

Synapse_t
synapse_with_index(Network_t network, const int index)
{
	if (network
	&& (0 <= index)
	&& (index < network->_number_of_synapse))
	{
		return network->_synapses[index];
	}

	return NULL;
}

void
delete_network(Network_t network)
{
    if (network != NULL)
    {
        int i;
        if (network->_number_of_synapse > 0)
        {
            for (i = 0; i < network->_number_of_synapse; ++i)
            {
                delete_synapse(network->_synapses[i]);
            }

            free(network->_synapses);
        }

        if (network->_layers != NULL)
        {
            for (i = 0; i < network->_number_of_layer; ++i)
            {
                delete_layer(network->_layers[i]);
            }

            free(network->_layers);
        }

        if (network->_output)
        {
            free(network->_output);
        }

        free(network);
    }
}

void
set_network_input(Network_t network, const int number_of_input, const double *in)
{
    if (in != NULL )
    {
        Layer_t input_layer = layer(network, 0);

        set_layer_input(input_layer, number_of_input, in);
    }
}

Network_t
new_network_from_context(Context context)
{
    Context subcontext;
    Network_t _network = NULL;
    int index = 0, number_of_outputs;

    if (!context || !is_node_with_name(context, "network"))
    {
        BRAIN_CRITICAL("Network", "<%s:%d> Context is not valid !\n",  __FILE__, __LINE__);
        return NULL;
    }

    _network = (Network_t)malloc(sizeof(Network));
    _network->_number_of_layer   = get_number_of_node_with_name(context, "layer");
    _network->_number_of_synapse = get_number_of_node_with_name(context, "connect");
	_network->_is_trained        = node_get_int(context, "trained", 0);

    BRAIN_INFO("Number of layer : %d, Number of synapse : %d", _network->_number_of_layer,
                                                                                 _network->_number_of_synapse);

    if (_network->_number_of_layer)
    {
        _network->_layers = (Layer_t *)malloc(_network->_number_of_layer * sizeof(Layer_t));

        for (index = 0; index < _network->_number_of_layer; ++index)
        {
            subcontext = get_node_with_name_and_index(context, "layer", index);

            if (subcontext)
            {
                _network->_layers[index] = new_layer_from_context(subcontext);
            }
        }
    }

    if (_network->_number_of_synapse)
    {
        _network->_synapses = (Synapse_t *)malloc(_network->_number_of_synapse * sizeof(Synapse_t));

        for (index = 0; index < _network->_number_of_synapse; ++index)
        {
            subcontext = get_node_with_name_and_index(context, "connect", index);

            if (subcontext)
            {
                _network->_synapses[index] = new_synapse_from_context(subcontext);
            }
        }
    }

    number_of_outputs = get_number_of_neuron(layer(_network, _network->_number_of_layer - 1));
    _network->_output = (double *)malloc( number_of_outputs * sizeof(double));
    memset(_network->_output, 0, number_of_outputs * sizeof(double));

    return _network;
}

int
get_number_of_layer(const Network_t network)
{
	if (network)
	{
		return network->_number_of_layer;
	}

	return 0;
}

int
get_number_of_synapse(const Network_t network)
{
	if (network)
	{
		return network->_number_of_synapse;
	}

	return 0;
}

void
dump_network(const Network_t network, const char* filename)
{
	int i;

	if (network && filename)
	{
		FILE* file = fopen(filename, "w");
		if (file)
		{
			fprintf(file, "<network\n");
			fprintf(file, " trained=\"%d\"", network->_is_trained);
			fprintf(file, ">\n");

			for (i = 0; i < network->_number_of_layer; ++i)
			{
				dump_layer(network->_layers[i], file);
			}
		
			fprintf(file, "\n");

			for (i = 0; i < network->_number_of_synapse;++i)
			{
				dump_synapse(network->_synapses[i], file);
			}

			fprintf(file, "</network>\n");
			fclose(file);
		}
	}
	else
	{
		BRAIN_CRITICAL("Please give a valid file for dumping network content");
	}
}
