#include "translator.h"
#include <string.h>
#include <stdlib.h>

void cb_datapoint(bionet_datapoint_t *datapoint)
{
    bionet_resource_t *resource = NULL;
    char *node_name = NULL;
    char *res_name = NULL;
    char id[3];
    unsigned long int adc_id, calib_id, pot_id;
    double constant;
    
    resource = bionet_datapoint_get_resource(datapoint);
    bionet_split_resource_name(bionet_resource_get_name(resource), NULL, NULL, &node_name, &res_name);

    // determine where the resource is coming from
    if(strcmp(node_name, "0") == 0)
    {
        // Check if it is a state resource update from DMM
        for(int i=0; i<16; i++)
        {
            if(strcmp(default_settings->state_names[i], res_name) == 0)
            {
                int8_t value;
                bionet_resource_get_int8(resource, &value, NULL);
                bionet_resource_set_int8(adc_state_resource[i], value, NULL);
                hab_report_datapoints(bionet_resource_get_node(translator_resource[0]));
                return;
            }
        }

        // Wasn't state resource so must be a calibration 

        // use resource name to find adc number (0-15)
        //ep = hsearch(e, FIND);
        //adc_id = (unsigned long int)ep->data;
        adc_id = 0;
    
        // use resource name to find calibration constant number (0-6)
        id[0] = res_name[19];
        id[1] = '\0';
        calib_id = strtol(id, NULL, 10);

        // get content from value
        bionet_resource_get_double(resource, &constant, NULL);

        // set calibration constant and update engineering value table
        set_calibration_const(adc_id, calib_id, constant);

        // update min and max resource value
        double buff = table[adc_id][0][ENG_VAL];
        bionet_resource_set_double(adc_range_resource[adc_id][ZERO_VOLT], buff, NULL);
        buff = table[adc_id][255][ENG_VAL];
        bionet_resource_set_double(adc_range_resource[adc_id][FIVE_VOLT], buff, NULL);
    }
    else if(strcmp(node_name, "potentiometers") == 0)
    {
        // extract pot number from resource name (0-15)
        //ep = hsearch(e, FIND);
        //pot_id = (unsigned long int)ep->data;
        pot_id = 0;
       
        // store the resource
        proxr_resource[pot_id] = resource;

        // pot value change? update translator hab to reflect it
        bionet_resource_get_double(resource, &constant, NULL);
        // have double of voltage find the corresponding cooked val and report it
        int volt_index = constant/VOLT_INC;
        bionet_resource_set_double(translator_resource[pot_id], table[pot_id][volt_index][ENG_VAL], NULL);
    }

    hab_report_datapoints(bionet_resource_get_node(translator_resource[0]));
}
