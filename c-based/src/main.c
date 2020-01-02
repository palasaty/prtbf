#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <protobuf2json.h>
#include "../proto/ComputationalLifeProtocol.pb-c.h"

#define FILE_NAME "complex.txt"

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

void ProvideOutletPressure()
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

       pressure.type = E_PROPERTY_TYPE__Providing;
       pressure.mmhg = 98;
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

void ProvideOutletFlowAndInletPressure()
{
       BoundaryConditionsListData condList = BOUNDARY_CONDITIONS_LIST_DATA__INIT;
       BoundaryConditionsData  aorta = BOUNDARY_CONDITIONS_DATA__INIT;
       BoundaryConditionData outlet = BOUNDARY_CONDITION_DATA__INIT;
       BoundaryConditionData inlet = BOUNDARY_CONDITION_DATA__INIT;
       PressureConditionData pressure = PRESSURE_CONDITION_DATA__INIT;
       FlowConditionData flow = FLOW_CONDITION_DATA__INIT;

       condList.n_boundaryconditions = 1;
       condList.boundaryconditions = calloc(condList.n_boundaryconditions, sizeof(BoundaryConditionsData*));
       if(condList.boundaryconditions == NULL){
               printf("Can't allocate memory");
               return;
       }
       condList.boundaryconditions[0] = (BoundaryConditionsData*)&aorta;

       pressure.type = E_PROPERTY_TYPE__Providing;
       pressure.mmhg = 98;

       flow.type = E_PROPERTY_TYPE__Providing;
       flow.ml_per_s = 4.76; 

       outlet.id = 1;
       outlet.name = "Aorta";
       outlet.flow = &flow;
       
       inlet.id = 2;
       inlet.name = "Aorta";
       inlet.pressure = &pressure;
       
       aorta.outlet = &outlet;
       aorta.inlet = &inlet;

	char *json;
        protobuf2json_string(
                &condList.base,
                JSON_COMPACT | JSON_REAL_PRECISION(3),
                &json, NULL, 0
        );
        printf("{\"BoundaryExchangeList\":%s}\n", json);

        free(json);

}

void RequestInletFlow()
{
       BoundaryConditionsListData condList = BOUNDARY_CONDITIONS_LIST_DATA__INIT;
       BoundaryConditionsData  aorta = BOUNDARY_CONDITIONS_DATA__INIT;
       BoundaryConditionData inlet = BOUNDARY_CONDITION_DATA__INIT;
       FlowConditionData flow = FLOW_CONDITION_DATA__INIT;

       condList.n_boundaryconditions = 1;
       condList.boundaryconditions = calloc(condList.n_boundaryconditions, sizeof(BoundaryConditionsData*));
       if(condList.boundaryconditions == NULL){
               printf("Can't allocate memory");
               return;
       }
       condList.boundaryconditions[0] = (BoundaryConditionsData*)&aorta;

       flow.type = E_PROPERTY_TYPE__Requested;

       inlet.id = 1;
       inlet.name = "Aorta";
       inlet.flow = &flow;

       aorta.inlet = &inlet;
     char *json;
     protobuf2json_string(
                &condList.base,
                JSON_COMPACT,
                &json, NULL, 0
     );
     printf("{\"BoundaryExchangeList\":%s}\n", json);

     free(json);

}

void ProvideInletFlow()
{
       BoundaryConditionsListData condList = BOUNDARY_CONDITIONS_LIST_DATA__INIT;
       BoundaryConditionsData  aorta = BOUNDARY_CONDITIONS_DATA__INIT;
       BoundaryConditionData inlet = BOUNDARY_CONDITION_DATA__INIT;
       FlowConditionData flow = FLOW_CONDITION_DATA__INIT;

       condList.n_boundaryconditions = 1;
       condList.boundaryconditions = calloc(condList.n_boundaryconditions, sizeof(BoundaryConditionsData*));
       if(condList.boundaryconditions == NULL){
               printf("Can't allocate memory");
               return;
       }
       condList.boundaryconditions[0] = (BoundaryConditionsData*)&aorta;

      flow.type = E_PROPERTY_TYPE__Requested;

      inlet.id = 1;
      inlet.name = "Aorta";
      inlet.flow = &flow;
      aorta.inlet = &inlet;

      char *json;
      protobuf2json_string(
                &condList.base,
                JSON_COMPACT,
                &json, NULL, 0
      );
      printf("{\"BoundaryExchangeList\":%s}\n", json);

      free(json);
}

void createComplexJson()
{
       BoundaryConditionsListData condList = BOUNDARY_CONDITIONS_LIST_DATA__INIT;
       
       condList.n_boundaryconditions = 2;
       condList.boundaryconditions = calloc(condList.n_boundaryconditions, sizeof(BoundaryConditionsData*));
       if(condList.boundaryconditions == NULL){
               printf("Can't allocate memory");
               return;
       }

       BoundaryConditionsData data[2];
       BoundaryConditionData outlet[2];
       BoundaryConditionData inlet[2]; 
       PressureConditionData pressure[2];
       FlowConditionData flow[2];
       SubstanceData substance[4];
       ConcentrationConditionData concentration[4];

       for (int i=0; i < condList.n_boundaryconditions; ++i){
		boundary_conditions_data__init(&data[i]);
		boundary_condition_data__init(&outlet[i]);
		boundary_condition_data__init(&inlet[i]);
		pressure_condition_data__init(&pressure[i]);
		flow_condition_data__init(&flow[i]);
		substance_data__init(&substance[2*i]);
		substance_data__init(&substance[2*i+1]);
		concentration_condition_data__init(&concentration[2*i]);
		concentration_condition_data__init(&concentration[2*i+1]);

		pressure[i].mmhg = 10;
		flow[i].ml_per_s = 20;
		concentration[2*i].mg_per_ml = 0.3;
		concentration[2*i + 1].mg_per_ml = 0.7;
		substance[2*i].name = "O2";
		substance[2*i+1].name = "CO2";
		substance[2*i].concentration = &concentration[2*i];
		substance[2*i+1].concentration = &concentration[2*i+1];

		inlet[i].id = 2*i + 1;
		inlet[i].name = "3D-Aorta";
		outlet[i].id = 2*i + 2;
		outlet[i].name = "Aortic arch";
		outlet[i].pressure = &pressure[i];
		outlet[i].flow = &flow[i];
		outlet[i].n_substance = 2;
		outlet[i].substance = calloc(outlet[i].n_substance, sizeof(SubstanceData*));
		outlet[i].substance[0] = &substance[2*i];		
		outlet[i].substance[1] = &substance[2*i+1];

		data[i].inlet = &inlet[i];
		data[i].outlet = &outlet[i];

       		condList.boundaryconditions[i] = &data[i];
       }

	char *json;
        protobuf2json_string(
                &condList.base,
                JSON_COMPACT | JSON_REAL_PRECISION(2),
                &json, NULL, 0
        );

	FILE *fp;
	fp = fopen(FILE_NAME, "w");
	if(fp == NULL){
		printf("Can't open/create file for writing ");
		return;
	}
        fprintf(fp, "{\"BoundaryExchangeList\":%s}\n", json);
	fclose(fp);
        free(json);
}

void printComplexJson()
{
	FILE *fp;
	fp = fopen(FILE_NAME, "r");
	if(fp == NULL) {
		printf("Can't open file for reading");
		return;
	}

	fseek( fp, 0L, SEEK_END );
	size_t size = ftell(fp);
	fseek( fp, 0L, SEEK_SET );
	if(!size){
		printf("bad file for printing");
		return;
	}
	char* json = malloc( size );
	fread( json, 1, size, fp );
	fclose(fp);

	printf("%s\n", json);
	free(json);
}

int main()
{
	printf("Start protobuf program\n");
	printf("Test json examples:\n\n");
	RequestOutletPressure();
	ProvideOutletPressure();
	ProvideOutletFlowAndInletPressure();
	March();
	RequestInletFlow();
        ProvideInletFlow();
	Stop();

	printf("Complex json exmple:\n\n");

	createComplexJson();
	printComplexJson();

	return 0;
}
