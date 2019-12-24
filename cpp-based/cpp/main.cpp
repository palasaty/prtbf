#include <fstream>
#include "ComputationalLifeProtocol.pb.h"
#include <google/protobuf/util/json_util.h>

void RequestOutletPressure()
{
  std::string json;
  // Create the main interchange objet
  ModelInterchangeData* pressure_request_exchange = new ModelInterchangeData();
  // We are interchanging boundary conditions
  BoundaryConditionsListData* boundary_conditions_list = new BoundaryConditionsListData();
  pressure_request_exchange->set_allocated_boundaryexchangelist(boundary_conditions_list);
  // Create a specific boundary exchange for the aorta
  BoundaryConditionsData* aorta_boundary = boundary_conditions_list->add_boundaryconditions();
  // Create the pressure request
  PressureConditionData* pressure_request = new PressureConditionData();
  pressure_request->set_type(ePropertyType::Requested);
  // Add it to an outlet
  BoundaryConditionData* outlet = new BoundaryConditionData();
  outlet->set_id(1);
  outlet->set_name("Aorta");
  outlet->set_allocated_pressure(pressure_request);
  // Add the outlet to the aorta boundary
  aorta_boundary->set_allocated_outlet(outlet);
  // Write this object out in JSON
  google::protobuf::util::JsonPrintOptions opts;
  opts.preserve_proto_field_names = true;
  bool ret = google::protobuf::util::MessageToJsonString(*pressure_request_exchange, &json, opts).ok();
  std::cout << json << "\n\n\n";
  std::ofstream myfile;
  myfile.open("1_RequestOutletPressure.json");
  myfile << json;
  myfile.close();
}

void ProvideOutletPressure()
{
  std::string json;
  // Create the main interchange objet
  ModelInterchangeData* pressure_provide_exchange = new ModelInterchangeData();
  // We are interchanging boundary conditions
  BoundaryConditionsListData* boundary_conditions_list = new BoundaryConditionsListData();
  pressure_provide_exchange->set_allocated_boundaryexchangelist(boundary_conditions_list);
  // Create a specific boundary exchange for the aorta
  BoundaryConditionsData* aorta_boundary = boundary_conditions_list->add_boundaryconditions();
  // Create the pressure request
  PressureConditionData* provided_pressure = new PressureConditionData();
  provided_pressure->set_type(ePropertyType::Providing);
  provided_pressure->set_mmhg(98);
  // Add it to an outlet
  BoundaryConditionData* outlet = new BoundaryConditionData();
  outlet->set_id(1);
  outlet->set_name("Aorta");
  outlet->set_allocated_pressure(provided_pressure);
  // Add the outlet to the aorta boundary
  aorta_boundary->set_allocated_outlet(outlet);
  // Write this object out in JSON
  google::protobuf::util::JsonPrintOptions opts;
  opts.preserve_proto_field_names = true;
  bool ret = google::protobuf::util::MessageToJsonString(*pressure_provide_exchange, &json, opts).ok();
  std::cout << json << "\n\n\n";
  std::ofstream myfile;
  myfile.open("2_ProvideOutletPressure.json");
  myfile << json;
  myfile.close();
}

void ProvideOutletFlowAndInletPressure()
{
  std::string json;
  // Create the main interchange objet
  ModelInterchangeData* pressure_provide_exchange = new ModelInterchangeData();
  // We are interchanging boundary conditions
  BoundaryConditionsListData* boundary_conditions_list = new BoundaryConditionsListData();
  pressure_provide_exchange->set_allocated_boundaryexchangelist(boundary_conditions_list);
  // Create a specific boundary exchange for the aorta
  BoundaryConditionsData* aorta_boundary = boundary_conditions_list->add_boundaryconditions();
  // Create the outlet flow
  FlowConditionData* provided_flow = new FlowConditionData();
  provided_flow->set_type(ePropertyType::Providing);
  provided_flow->set_ml_per_s(4.76);
  // Add it to an outlet
  BoundaryConditionData* outlet = new BoundaryConditionData();
  outlet->set_id(1);
  outlet->set_name("Aorta");
  outlet->set_allocated_flow(provided_flow);
  // Add the outlet to the aorta boundary
  aorta_boundary->set_allocated_outlet(outlet);
  // Create the inlet pressure
  PressureConditionData* provided_pressure = new PressureConditionData();
  provided_pressure->set_type(ePropertyType::Providing);
  provided_pressure->set_mmhg(98);
  // Add it to an inlet
  BoundaryConditionData* inlet = new BoundaryConditionData();
  inlet->set_id(2);
  inlet->set_name("Aorta");
  inlet->set_allocated_pressure(provided_pressure);
  // Add the outlet to the aorta boundary
  aorta_boundary->set_allocated_inlet(inlet);
  // Write this object out in JSON
  google::protobuf::util::JsonPrintOptions opts;
  opts.preserve_proto_field_names = true;
  bool ret = google::protobuf::util::MessageToJsonString(*pressure_provide_exchange, &json, opts).ok();
  std::cout << json << "\n\n\n";
  std::ofstream myfile;
  myfile.open("3_ProvideOutletFlowAndInletPressure.json");
  myfile << json;
  myfile.close();
}

void March()
{
  std::string json;
  // Create the main interchange objet
  ModelInterchangeData* march_exchange = new ModelInterchangeData();
  MarchData* march = new MarchData();
  march->set_duration_s(0.001);
  march_exchange->set_allocated_march(march);
  // Write this object out in JSON
  google::protobuf::util::JsonPrintOptions opts;
  opts.preserve_proto_field_names = true;
  bool ret = google::protobuf::util::MessageToJsonString(*march_exchange, &json, opts).ok();
  std::cout << json << "\n\n\n";
  std::ofstream myfile;
  myfile.open("4_March.json");
  myfile << json;
  myfile.close();
}

void RequestInletFlow()
{
  std::string json;
  // Create the main interchange objet
  ModelInterchangeData* flow_request_exchange = new ModelInterchangeData();
  // We are interchanging boundary conditions
  BoundaryConditionsListData* boundary_conditions_list = new BoundaryConditionsListData();
  flow_request_exchange->set_allocated_boundaryexchangelist(boundary_conditions_list);
  // Create a specific boundary exchange for the aorta
  BoundaryConditionsData* aorta_boundary = boundary_conditions_list->add_boundaryconditions();
  // Create the pressure request
  FlowConditionData* flow_request = new FlowConditionData();
  flow_request->set_type(ePropertyType::Requested);
  // Add it to an outlet
  BoundaryConditionData* inlet = new BoundaryConditionData();
  inlet->set_id(1);
  inlet->set_name("Aorta");
  inlet->set_allocated_flow(flow_request);
  // Add the outlet to the aorta boundary
  aorta_boundary->set_allocated_outlet(inlet);
  // Write this object out in JSON
  google::protobuf::util::JsonPrintOptions opts;
  opts.preserve_proto_field_names = true;
  bool ret = google::protobuf::util::MessageToJsonString(*flow_request_exchange, &json, opts).ok();
  std::cout << json << "\n\n\n";
  std::ofstream myfile;
  myfile.open("5_RequestInletFlow.json");
  myfile << json;
  myfile.close();
}

void ProvideInletFlow()
{
  std::string json;
  // Create the main interchange objet
  ModelInterchangeData* flow_request_exchange = new ModelInterchangeData();
  // We are interchanging boundary conditions
  BoundaryConditionsListData* boundary_conditions_list = new BoundaryConditionsListData();
  flow_request_exchange->set_allocated_boundaryexchangelist(boundary_conditions_list);
  // Create a specific boundary exchange for the aorta
  BoundaryConditionsData* aorta_boundary = boundary_conditions_list->add_boundaryconditions();
  // Create the pressure request
  FlowConditionData* flow_value = new FlowConditionData();
  flow_value->set_type(ePropertyType::Providing);
  flow_value->set_ml_per_s(5.34);
  // Add it to an outlet
  BoundaryConditionData* inlet = new BoundaryConditionData();
  inlet->set_id(1);
  inlet->set_name("Aorta");
  inlet->set_allocated_flow(flow_value);
  // Add the outlet to the aorta boundary
  aorta_boundary->set_allocated_outlet(inlet);
  // Write this object out in JSON
  google::protobuf::util::JsonPrintOptions opts;
  opts.preserve_proto_field_names = true;
  bool ret = google::protobuf::util::MessageToJsonString(*flow_request_exchange, &json, opts).ok();
  std::cout << json << "\n\n\n";
  std::ofstream myfile;
  myfile.open("6_ProvideInletFlow.json");
  myfile << json;
  myfile.close();
}

void Stop()
{
  std::string json;
  // Create the main interchange objet
  ModelInterchangeData* stop_exchange = new ModelInterchangeData();
  StopData* stop = new StopData();\
  stop_exchange->set_allocated_stop(stop);
  // Write this object out in JSON
  google::protobuf::util::JsonPrintOptions opts;
  opts.preserve_proto_field_names = true;
  bool ret = google::protobuf::util::MessageToJsonString(*stop_exchange, &json, opts).ok();
  std::cout << json << "\n\n\n";
  std::ofstream myfile;
  myfile.open("7_Stop.json");
  myfile << json;
  myfile.close();
}

int main(int argc, char* argv[])
{
  RequestOutletPressure();
  ProvideOutletPressure();
  ProvideOutletFlowAndInletPressure();
  March();
  RequestInletFlow();
  ProvideInletFlow();
  Stop();
}