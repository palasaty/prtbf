#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <protobuf2json.h>
#include "../proto/ComputationalLifeProtocol.pb-c.h"


void March()
{
	MarchData march = MARCH_DATA__INIT;

	march.duration_s = 0.01;
	char *json = NULL;
	protobuf2json_string(
		&march.base, 
		JSON_COMPACT, 
		&json, NULL, 0
	);
	printf("{\"March\": %s}\n", json);

	free(json);
}

void Stop()
{
        StopData stop= STOP_DATA__INIT;

        char *json = NULL;
        protobuf2json_string(
                &stop.base,
                JSON_COMPACT,
                &json, NULL, 0
        );
        printf("{\"Stop\":%s}\n", json);

        free(json);
}

void RequestOutletPressure()
{
	BoundaryConditionsListData condList = BOUNDARY_CONDITIONS_LIST_DATA__INIT;
	BoundaryConditionsData  aorta = BOUNDARY_CONDITIONS_DATA__INIT;
	BoundaryConditionData outlet = BOUNDARY_CONDITION_DATA__INIT;
	PressureConditionData pressure = PRESSURE_CONDITION_DATA__INIT;

	condList.n_boundaryconditions = 1;
	condList.boundaryconditions = calloc(condList.n_boundaryconditions, sizeof(BoundaryConditionsData*));
  	if(condList.boundaryconditions == NULL){
		printf("Can't allocate memory");
		return;
	}	
	condList.boundaryconditions[0] = (BoundaryConditionsData*)&aorta;

	pressure.type = E_PROPERTY_TYPE__Requested;
	outlet.id = 1;
	outlet.name = "Aorta";
	outlet.pressure = &pressure;
	aorta.outlet = &outlet;

	char *json;
        protobuf2json_string(
                &condList.base,
                JSON_COMPACT,
                &json, NULL, 0
        );
        printf("{\"BoundaryExchangeList\":%s}\n", json);

        free(json);

}

int main()
{
	printf("Start protobuf program\n");
	RequestOutletPressure();
	March();
	Stop();
	return 0;
}
