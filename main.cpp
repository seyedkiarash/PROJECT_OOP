

/////////////////////////////////////
//full1

 // #include <iostream>
 // #include <vector>
 // #include <string>
 // #include <stdexcept>
 // #include <map>
 // #include <cmath>
 // #include <regex>
 // #include <algorithm>
 // #include <iomanip>
 // #include <fstream>
 // #include <sstream>
 // #include <set>
 //
 // // کتابخانه Eigen برای عملیات ماتریسی
 // // اطمینان حاصل کنید که هدرهای Eigen در مسیر پروژه شما قرار دارند.
 // #include "Eigen/Dense"
 //
 // #ifndef M_PI
 // #define M_PI 3.14159265358979323846
 // #endif
 //
 // using namespace std;
 //
 // // ==========================================================================================
 // // ||                                    بخش مدل (Model)                                   ||
 // // ==========================================================================================
 //
 // class Node {
 // private:
 //     double voltage;
 //     string name;
 //     double previousVoltage;
 //
 // public:
 //     Node(const string &name, double voltage = 0.0, double prev_voltage = 0.0)
 //             : voltage(voltage), name(name), previousVoltage(prev_voltage) {}
 //
 //     string getName() const { return name; }
 //     void setName(const string& newName) { this->name = newName; }
 //     double getVoltage() const { return voltage; }
 //     void setVoltage(double v) { voltage = v; }
 //     double getPreviousVoltage() const { return previousVoltage; }
 //     void setPreviousVoltage(double pv) { previousVoltage = pv; }
 //     void updateVoltageForNextStep() { previousVoltage = voltage; }
 //     bool isGround() const { return name == "0" || name == "GND" || name == "gnd"; }
 // };
 //
 // class Element {
 // protected:
 //     Node *node1, *node2;
 //     string name;
 // public:
 //     Element(Node* n1, Node* n2, const string &name) : node1(n1), node2(n2), name(name) {
 //         if (!n1 || !n2) throw std::invalid_argument("Element nodes cannot be null for element: " + name);
 //     }
 //     virtual ~Element() = default;
 //     string getName() const { return name; }
 //     Node* getNode1() const { return node1; }
 //     Node* getNode2() const { return node2; }
 //     virtual string getType() const = 0;
 //     virtual string getValueAsString() const {return to_string(getValue()); }
 //     virtual double getValue() const { return 0.0; }
 //     virtual double getCurrent() const { return 0.0; }
 //     virtual void setCurrent(double current) { (void)current; }
 //     virtual void updateValue(double time) { (void)time; }
 // };
 //
 // class Resistor : public Element {
 // private: double resistance;
 // public:
 //     Resistor(Node* n1, Node* n2, const string &name, double res) : Element(n1, n2, name) {
 //         if (res <= 0) throw std::invalid_argument("Resistance must be positive for " + name);
 //         this->resistance = res;
 //     }
 //     string getType() const override { return "R"; }
 //     double getValue() const override { return resistance; }
 //     string getValueAsString() const override { return to_string(resistance); }
 //     double getCurrent() const override {
 //         if (!node1 || !node2) return 0.0;
 //         return (node1->getVoltage() - node2->getVoltage()) / resistance;
 //     }
 // };
 //
 // class BaseSource : public Element {
 // public:
 //     enum SourceType { DC, SIN, PULSE };
 // protected:
 //     double value;
 //     SourceType sourceType;
 //     double currentThroughSource;
 //
 //     double dcOffset, amplitude, frequency;
 //     double initialValue, pulsedValue, delayTime, riseTime, fallTime, onTime, period;
 // public:
 //     BaseSource(Node* n1, Node* n2, const string& name, double val)
 //         : Element(n1, n2, name), value(val), sourceType(DC), currentThroughSource(0.0) {}
 //
 //     void setSinParams(double offset, double amp, double freq) {
 //         sourceType = SIN;
 //         dcOffset = offset;
 //         amplitude = amp;
 //         frequency = freq;
 //     }
 //
 //     void setPulseParams(double v1, double v2, double td, double tr, double tf, double ton, double tper) {
 //         sourceType = PULSE;
 //         initialValue = v1; pulsedValue = v2; delayTime = td;
 //         riseTime = tr > 0 ? tr : 1e-12; // Avoid division by zero
 //         fallTime = tf > 0 ? tf : 1e-12;
 //         onTime = ton; period = tper;
 //     }
 //
 //     void updateValue(double time) override {
 //         if (sourceType == SIN) {
 //             value = dcOffset + amplitude * sin(2 * M_PI * frequency * time);
 //         } else if (sourceType == PULSE) {
 //             if (time < delayTime) {
 //                 value = initialValue;
 //                 return;
 //             }
 //             double timeInCycle = fmod(time - delayTime, period);
 //             if (timeInCycle <= riseTime) {
 //                 value = initialValue + (pulsedValue - initialValue) * (timeInCycle / riseTime);
 //             } else if (timeInCycle <= riseTime + onTime) {
 //                 value = pulsedValue;
 //             } else if (timeInCycle <= riseTime + onTime + fallTime) {
 //                 value = pulsedValue - (pulsedValue - initialValue) * ((timeInCycle - (riseTime + onTime)) / fallTime);
 //             } else {
 //                 value = initialValue;
 //             }
 //         }
 //     }
 //
 //     double getValue() const override { return value; }
 //     void setValue(double val) { value = val; sourceType = DC; }
 //     double getCurrent() const override { return currentThroughSource; }
 //     void setCurrent(double current) override { this->currentThroughSource = current; }
 // };
 //
 // class VoltageSource : public BaseSource {
 // public:
 //     VoltageSource(Node* n1, Node* n2, const string &name, double dc_val)
 //         : BaseSource(n1, n2, name, dc_val) {}
 //     string getType() const override { return "V"; }
 // };
 //
 // class CurrentSource : public BaseSource {
 // public:
 //     CurrentSource(Node* n1, Node* n2, const string &name, double dc_val)
 //         : BaseSource(n1, n2, name, dc_val) {}
 //     string getType() const override { return "I"; }
 // };
 //
 //
 // class Capacitor : public Element {
 // private: double capacitance;
 // public:
 //     Capacitor(Node* n1, Node* n2, const string &name, double cap) : Element(n1, n2, name) {
 //         if (cap <= 0) throw std::invalid_argument("Capacitance must be positive for " + name);
 //         this->capacitance = cap;
 //     }
 //     string getType() const override { return "C"; }
 //     double getValue() const override { return capacitance; }
 //     string getValueAsString() const override { return to_string(capacitance); }
 // };
 //
 // class Inductor : public Element {
 // private: double inductance, current, previousCurrent;
 // public:
 //     Inductor(Node* n1, Node* n2, const string &name, double ind)
 //             : Element(n1, n2, name), inductance(ind), current(0.0), previousCurrent(0.0) {
 //         if (ind <= 0) throw std::invalid_argument("Inductance must be positive for " + name);
 //     }
 //     string getType() const override { return "L"; }
 //     double getValue() const override { return inductance; }
 //     string getValueAsString() const override { return to_string(inductance); }
 //     double getCurrent() const override { return current; }
 //     void setCurrent(double c) override { current = c; }
 //     double getPreviousCurrent() const { return previousCurrent; }
 //     void updateCurrentForNextStep() { previousCurrent = current; }
 // };
 //
 // class IdealDiode : public Element {
 // public: enum State { ON, OFF };
 // private: double forwardVoltage; State currentState; double current; string model;
 // public:
 //     IdealDiode(Node* n1, Node* n2, const string& name, const string& model_str, double vf = 0.7)
 //             : Element(n1, n2, name), forwardVoltage(vf), currentState(OFF), current(0.0), model(model_str) {
 //         if (vf < 0) throw std::invalid_argument("Diode forward voltage must be non-negative for " + name);
 //     }
 //     string getType() const override { return "D"; }
 //     string getValueAsString() const override { return model; }
 //     double getForwardVoltage() const { return forwardVoltage; }
 //     State getState() const { return currentState; }
 //     void setState(State state) { currentState = state; }
 //     double getCurrent() const override { return current; }
 //     void setCurrent(double c) override { current = c; }
 // };
 //
 // class VCVS : public Element {
 // private: Node* controlNode1; Node* controlNode2; double gain; double current;
 // public:
 //     VCVS(Node* n1, Node* n2, const string& name, Node* cn1, Node* cn2, double g)
 //             : Element(n1, n2, name), controlNode1(cn1), controlNode2(cn2), gain(g), current(0.0) {}
 //     string getType() const override { return "E"; }
 //     double getValue() const override { return gain; }
 //     string getValueAsString() const override {
 //         stringstream ss;
 //         ss << controlNode1->getName() << " " << controlNode2->getName() << " " << to_string(gain);
 //         return ss.str();
 //     }
 //     Node* getControlNode1() const { return controlNode1; }
 //     Node* getControlNode2() const { return controlNode2; }
 //     double getCurrent() const override { return current; }
 //     void setCurrent(double c) override { current = c; }
 // };
 //
 // class VCCS : public Element {
 // private: Node* controlNode1; Node* controlNode2; double gain;
 // public:
 //     VCCS(Node* n1, Node* n2, const string& name, Node* cn1, Node* cn2, double g)
 //             : Element(n1, n2, name), controlNode1(cn1), controlNode2(cn2), gain(g) {}
 //     string getType() const override { return "G"; }
 //     double getValue() const override { return gain; }
 //     string getValueAsString() const override {
 //         stringstream ss;
 //         ss << controlNode1->getName() << " " << controlNode2->getName() << " " << to_string(gain);
 //         return ss.str();
 //     }
 //     Node* getControlNode1() const { return controlNode1; }
 //     Node* getControlNode2() const { return controlNode2; }
 // };
 //
 // class CCVS : public Element {
 // private: string controlVoltageSourceName; double gain; double current;
 // public:
 //     CCVS(Node* n1, Node* n2, const string& name, const string& cvs_name, double g)
 //             : Element(n1, n2, name), controlVoltageSourceName(cvs_name), gain(g), current(0.0) {}
 //     string getType() const override { return "H"; }
 //     double getValue() const override { return gain; }
 //     string getValueAsString() const override {
 //         stringstream ss;
 //         ss << controlVoltageSourceName << " " << to_string(gain);
 //         return ss.str();
 //     }
 //     string getControlVoltageSourceName() const { return controlVoltageSourceName; }
 //     double getCurrent() const override { return current; }
 //     void setCurrent(double c) override { current = c; }
 // };
 //
 // class CCCS : public Element {
 // private: string controlVoltageSourceName; double gain;
 // public:
 //     CCCS(Node* n1, Node* n2, const string& name, const string& cvs_name, double g)
 //             : Element(n1, n2, name), controlVoltageSourceName(cvs_name), gain(g) {}
 //     string getType() const override { return "F"; }
 //     double getValue() const override { return gain; }
 //     string getValueAsString() const override {
 //         stringstream ss;
 //         ss << controlVoltageSourceName << " " << to_string(gain);
 //         return ss.str();
 //     }
 //     string getControlVoltageSourceName() const { return controlVoltageSourceName; }
 // };
 //
 // class MakingMNA {
 // private:
 //     vector<Node *> allNodesInCircuit;
 //     vector<Element *> elementsInCircuit;
 //     Node *groundNodeRef;
 //     map<Node *, int> nodeToIndexMap;
 //     vector<Node *> orderedNonGroundNodes;
 //     map<VoltageSource *, int> vsToIndexMap;
 //     vector<VoltageSource *> orderedVoltageSources;
 //     map<Inductor *, int> inductorToIndexMap;
 //     vector<Inductor *> orderedInductors;
 //     map<IdealDiode *, int> idealDiodeToIndexMap;
 //     vector<IdealDiode *> orderedIdealDiodes;
 //     map<VCVS *, int> vcvsToIndexMap;
 //     vector<VCVS *> orderedVCVS;
 //     map<CCVS *, int> ccvsToIndexMap;
 //     vector<CCVS *> orderedCCVS;
 //     double timeStep_h;
 //
 //     void buildSystemMaps() {
 //         nodeToIndexMap.clear(); orderedNonGroundNodes.clear();
 //         vsToIndexMap.clear(); orderedVoltageSources.clear();
 //         inductorToIndexMap.clear(); orderedInductors.clear();
 //         idealDiodeToIndexMap.clear(); orderedIdealDiodes.clear();
 //         vcvsToIndexMap.clear(); orderedVCVS.clear();
 //         ccvsToIndexMap.clear(); orderedCCVS.clear();
 //
 //         groundNodeRef = nullptr;
 //         for (Node *n: allNodesInCircuit) if (n->isGround()) { groundNodeRef = n; break; }
 //         if (!groundNodeRef && !allNodesInCircuit.empty()) throw std::runtime_error("Error: Ground node not detected in the circuit.");
 //
 //         int nodeIdx = 0;
 //         for (Node *node: allNodesInCircuit) {
 //             if (!node->isGround()) {
 //                 orderedNonGroundNodes.push_back(node);
 //                 nodeToIndexMap[node] = nodeIdx++;
 //             }
 //         }
 //
 //         for (Element *elem: elementsInCircuit) {
 //             if (auto vs = dynamic_cast<VoltageSource *>(elem)) orderedVoltageSources.push_back(vs);
 //             else if (auto ind = dynamic_cast<Inductor *>(elem)) orderedInductors.push_back(ind);
 //             else if (auto id = dynamic_cast<IdealDiode *>(elem)) orderedIdealDiodes.push_back(id);
 //             else if (auto vcvs = dynamic_cast<VCVS *>(elem)) orderedVCVS.push_back(vcvs);
 //             else if (auto ccvs = dynamic_cast<CCVS *>(elem)) orderedCCVS.push_back(ccvs);
 //         }
 //
 //         for (size_t i = 0; i < orderedVoltageSources.size(); ++i) vsToIndexMap[orderedVoltageSources[i]] = i;
 //         for (size_t i = 0; i < orderedInductors.size(); ++i) inductorToIndexMap[orderedInductors[i]] = i;
 //         for (size_t i = 0; i < orderedIdealDiodes.size(); ++i) idealDiodeToIndexMap[orderedIdealDiodes[i]] = i;
 //         for (size_t i = 0; i < orderedVCVS.size(); ++i) vcvsToIndexMap[orderedVCVS[i]] = i;
 //         for (size_t i = 0; i < orderedCCVS.size(); ++i) ccvsToIndexMap[orderedCCVS[i]] = i;
 //     }
 //
 // public:
 //     MakingMNA(double h = -1.0) : groundNodeRef(nullptr), timeStep_h(h) {}
 //
 //     void setCircuitData(const vector<Node*>& nodes, const vector<Element*>& elements){
 //         this->allNodesInCircuit = nodes;
 //         this->elementsInCircuit = elements;
 //     }
 //
 //     void setTimeStep(double h) { this->timeStep_h = h; }
 //     const vector<Node*>& getOrderedNonGroundNodes() const { return orderedNonGroundNodes; }
 //     const vector<Element*>& getAllElements() const { return elementsInCircuit; }
 //     const vector<Node*>& getAllNodesInCircuit() const { return allNodesInCircuit; }
 //     const vector<VoltageSource*>& getOrderedVoltageSources() const { return orderedVoltageSources; }
 //     const vector<Inductor*>& getOrderedInductors() const { return orderedInductors; }
 //     const vector<IdealDiode*>& getOrderedIdealDiodes() const { return orderedIdealDiodes; }
 //     const vector<VCVS*>& getOrderedVCVS() const { return orderedVCVS; }
 //     const vector<CCVS*>& getOrderedCCVS() const { return orderedCCVS; }
 //
 //     Eigen::MatrixXd getSystemMatrixA(bool isDCAnalysis = false) {
 //         buildSystemMaps();
 //
 //         int numNonGroundNodes = orderedNonGroundNodes.size();
 //         int numVS = orderedVoltageSources.size();
 //         int numL = orderedInductors.size();
 //         int numD = orderedIdealDiodes.size();
 //         int numVCVS = orderedVCVS.size();
 //         int numCCVS = orderedCCVS.size();
 //         int systemSize = numNonGroundNodes + numVS + numL + numD + numVCVS + numCCVS;
 //
 //         if (systemSize == 0) return Eigen::MatrixXd(0,0);
 //         Eigen::MatrixXd A = Eigen::MatrixXd::Zero(systemSize, systemSize);
 //
 //         for (Element* elem : elementsInCircuit) {
 //             Node* n1 = elem->getNode1();
 //             Node* n2 = elem->getNode2();
 //
 //             if (auto res = dynamic_cast<Resistor*>(elem)) {
 //                 double g = 1.0 / res->getValue();
 //                 if (!n1->isGround()) A(nodeToIndexMap.at(n1), nodeToIndexMap.at(n1)) += g;
 //                 if (!n2->isGround()) A(nodeToIndexMap.at(n2), nodeToIndexMap.at(n2)) += g;
 //                 if (!n1->isGround() && !n2->isGround()) {
 //                     A(nodeToIndexMap.at(n1), nodeToIndexMap.at(n2)) -= g;
 //                     A(nodeToIndexMap.at(n2), nodeToIndexMap.at(n1)) -= g;
 //                 }
 //             }
 //             else if (auto cap = dynamic_cast<Capacitor*>(elem)) {
 //                 if (!isDCAnalysis) {
 //                     if (this->timeStep_h <= 0) throw std::runtime_error("Time step h is not set for capacitor " + cap->getName());
 //                     double g = cap->getValue() / this->timeStep_h;
 //                     if (!n1->isGround()) A(nodeToIndexMap.at(n1), nodeToIndexMap.at(n1)) += g;
 //                     if (!n2->isGround()) A(nodeToIndexMap.at(n2), nodeToIndexMap.at(n2)) += g;
 //                     if (!n1->isGround() && !n2->isGround()) {
 //                         A(nodeToIndexMap.at(n1), nodeToIndexMap.at(n2)) -= g;
 //                         A(nodeToIndexMap.at(n2), nodeToIndexMap.at(n1)) -= g;
 //                     }
 //                 }
 //             }
 //             else if (auto vs = dynamic_cast<VoltageSource*>(elem)) {
 //                 int i = numNonGroundNodes + vsToIndexMap.at(vs);
 //                 if (!n1->isGround()) { A(nodeToIndexMap.at(n1), i) += 1.0; A(i, nodeToIndexMap.at(n1)) += 1.0; }
 //                 if (!n2->isGround()) { A(nodeToIndexMap.at(n2), i) -= 1.0; A(i, nodeToIndexMap.at(n2)) -= 1.0; }
 //             }
 //             else if (auto ind = dynamic_cast<Inductor*>(elem)) {
 //                 int i = numNonGroundNodes + numVS + inductorToIndexMap.at(ind);
 //                 if (isDCAnalysis) {
 //                     if (!n1->isGround()) A(i, nodeToIndexMap.at(n1)) += 1.0;
 //                     if (!n2->isGround()) A(i, nodeToIndexMap.at(n2)) -= 1.0;
 //                 } else {
 //                     double l_div_h = ind->getValue() / this->timeStep_h;
 //                     if (!n1->isGround()) { A(nodeToIndexMap.at(n1), i) += 1.0; A(i, nodeToIndexMap.at(n1)) += 1.0; }
 //                     if (!n2->isGround()) { A(nodeToIndexMap.at(n2), i) -= 1.0; A(i, nodeToIndexMap.at(n2)) -= 1.0; }
 //                     A(i, i) -= l_div_h;
 //                 }
 //             }
 //             else if (auto diode = dynamic_cast<IdealDiode*>(elem)) {
 //                 int i = numNonGroundNodes + numVS + numL + idealDiodeToIndexMap.at(diode);
 //                 if (diode->getState() == IdealDiode::ON) {
 //                     if (!n1->isGround()) { A(nodeToIndexMap.at(n1), i) += 1.0; A(i, nodeToIndexMap.at(n1)) += 1.0; }
 //                     if (!n2->isGround()) { A(nodeToIndexMap.at(n2), i) -= 1.0; A(i, nodeToIndexMap.at(n2)) -= 1.0; }
 //                 } else {
 //                     A(i, i) = 1.0;
 //                 }
 //             }
 //             else if (auto vccs = dynamic_cast<VCCS*>(elem)) {
 //                 double g = vccs->getValue();
 //                 Node* cn1 = vccs->getControlNode1(); Node* cn2 = vccs->getControlNode2();
 //                 if (!n1->isGround() && !cn1->isGround()) A(nodeToIndexMap.at(n1), nodeToIndexMap.at(cn1)) += g;
 //                 if (!n1->isGround() && !cn2->isGround()) A(nodeToIndexMap.at(n1), nodeToIndexMap.at(cn2)) -= g;
 //                 if (!n2->isGround() && !cn1->isGround()) A(nodeToIndexMap.at(n2), nodeToIndexMap.at(cn1)) -= g;
 //                 if (!n2->isGround() && !cn2->isGround()) A(nodeToIndexMap.at(n2), nodeToIndexMap.at(cn2)) += g;
 //             }
 //             else if (auto cccs = dynamic_cast<CCCS*>(elem)) {
 //                 double gain = cccs->getValue();
 //                 VoltageSource* ctrl_vs = nullptr;
 //                 for(auto* vs_ptr : orderedVoltageSources) if(vs_ptr->getName() == cccs->getControlVoltageSourceName()) ctrl_vs = vs_ptr;
 //                 if (!ctrl_vs) throw std::runtime_error("Error: Dependent source '" + cccs->getName() + "' has an undefined control element '" + cccs->getControlVoltageSourceName() + "'.");
 //                 int ctrl_i = numNonGroundNodes + vsToIndexMap.at(ctrl_vs);
 //                 if (!n1->isGround()) A(nodeToIndexMap.at(n1), ctrl_i) += gain;
 //                 if (!n2->isGround()) A(nodeToIndexMap.at(n2), ctrl_i) -= gain;
 //             }
 //             else if (auto vcvs = dynamic_cast<VCVS*>(elem)) {
 //                 int i = numNonGroundNodes + numVS + numL + numD + vcvsToIndexMap.at(vcvs);
 //                 Node* cn1 = vcvs->getControlNode1(); Node* cn2 = vcvs->getControlNode2();
 //                 double gain = vcvs->getValue();
 //                 if (!n1->isGround()) { A(nodeToIndexMap.at(n1), i) += 1.0; A(i, nodeToIndexMap.at(n1)) += 1.0; }
 //                 if (!n2->isGround()) { A(nodeToIndexMap.at(n2), i) -= 1.0; A(i, nodeToIndexMap.at(n2)) -= 1.0; }
 //                 if (!cn1->isGround()) A(i, nodeToIndexMap.at(cn1)) -= gain;
 //                 if (!cn2->isGround()) A(i, nodeToIndexMap.at(cn2)) += gain;
 //             }
 //             else if (auto ccvs = dynamic_cast<CCVS*>(elem)) {
 //                 int i = numNonGroundNodes + numVS + numL + numD + numVCVS + ccvsToIndexMap.at(ccvs);
 //                 double gain = ccvs->getValue();
 //                 VoltageSource* ctrl_vs = nullptr;
 //                 for(auto* vs_ptr : orderedVoltageSources) if(vs_ptr->getName() == ccvs->getControlVoltageSourceName()) ctrl_vs = vs_ptr;
 //                 if (!ctrl_vs) throw std::runtime_error("Error: Dependent source '" + ccvs->getName() + "' has an undefined control element '" + ccvs->getControlVoltageSourceName() + "'.");
 //                 int ctrl_i = numNonGroundNodes + vsToIndexMap.at(ctrl_vs);
 //                 if (!n1->isGround()) {A(i, nodeToIndexMap.at(n1)) += 1.0;} else {A(i, nodeToIndexMap.at(n1))=0;};
 //                 if (!n2->isGround()) {A(i, nodeToIndexMap.at(n2)) -= 1.0;} else {A(i, nodeToIndexMap.at(n2))=0;};
 //                 A(i, ctrl_i) -= gain;
 //             }
 //         }
 //         return A;
 //     }
 //
 //     Eigen::VectorXd getSystemVectorZ(bool isDCAnalysis = false, double time = 0.0) {
 //         int systemSize = orderedNonGroundNodes.size() + orderedVoltageSources.size() + orderedInductors.size() + orderedIdealDiodes.size() + orderedVCVS.size() + orderedCCVS.size();
 //         if (systemSize == 0) return Eigen::VectorXd(0);
 //         Eigen::VectorXd Z = Eigen::VectorXd::Zero(systemSize);
 //
 //         for (Element* elem : elementsInCircuit) {
 //              if (auto cs = dynamic_cast<CurrentSource*>(elem)) {
 //                 cs->updateValue(time); // FIX 1: Update source value for transient
 //                 if (!cs->getNode1()->isGround()) Z(nodeToIndexMap.at(cs->getNode1())) -= cs->getValue();
 //                 if (!cs->getNode2()->isGround()) Z(nodeToIndexMap.at(cs->getNode2())) += cs->getValue();
 //             } else if (auto cap = dynamic_cast<Capacitor*>(elem)) {
 //                 if (!isDCAnalysis) {
 //                     if (this->timeStep_h <= 0) throw std::runtime_error("Time step h is not set for capacitor " + cap->getName());
 //                     double c_div_h = cap->getValue() / this->timeStep_h;
 //                     double v_n1_prev = cap->getNode1()->getPreviousVoltage();
 //                     double v_n2_prev = cap->getNode2()->getPreviousVoltage();
 //                     double i_eq_cap = c_div_h * (v_n1_prev - v_n2_prev);
 //                     if (!cap->getNode1()->isGround()) Z(nodeToIndexMap.at(cap->getNode1())) += i_eq_cap;
 //                     if (!cap->getNode2()->isGround()) Z(nodeToIndexMap.at(cap->getNode2())) -= i_eq_cap;
 //                 }
 //             }
 //         }
 //
 //         for (const auto& vs : orderedVoltageSources) {
 //             vs->updateValue(time); // FIX 1: Update source value for transient
 //             Z(orderedNonGroundNodes.size() + vsToIndexMap.at(vs)) = vs->getValue();
 //         }
 //         for (const auto& ind : orderedInductors) {
 //             int i = orderedNonGroundNodes.size() + orderedVoltageSources.size() + inductorToIndexMap.at(ind);
 //             if (!isDCAnalysis) {
 //                 Z(i) = - (ind->getValue() / this->timeStep_h) * ind->getPreviousCurrent();
 //             }
 //         }
 //         for (const auto& diode : orderedIdealDiodes) {
 //             int i = orderedNonGroundNodes.size() + orderedVoltageSources.size() + orderedInductors.size() + idealDiodeToIndexMap.at(diode);
 //             if (diode->getState() == IdealDiode::ON) Z(i) = diode->getForwardVoltage();
 //         }
 //         return Z;
 //     }
 // };
 //
 // class MNASolver {
 // public:
 //     MNASolver() {}
 //     Eigen::VectorXd solve(const Eigen::MatrixXd& A, const Eigen::VectorXd& Z) {
 //         if (A.rows() == 0 && Z.size() == 0) return Eigen::VectorXd(0);
 //         if (A.rows() != A.cols() || A.rows() != Z.size()) throw std::runtime_error("Matrix/vector dimensions incompatible.");
 //         if (A.rows() == 0) throw std::runtime_error("System of equations is empty.");
 //
 //         Eigen::PartialPivLU<Eigen::MatrixXd> lu(A);
 //         if (A.rows() > 0 && std::abs(lu.determinant()) < 1e-12) {
 //              throw std::runtime_error("Error: System matrix is singular or ill-conditioned. The circuit may not be solvable (e.g. floating nodes or voltage source loop).");
 //         }
 //         return lu.solve(Z);
 //     }
 //
 //     void updateCircuitState(const Eigen::VectorXd& X, MakingMNA& mnaCircuit) {
 //         const auto& nonGroundNodes = mnaCircuit.getOrderedNonGroundNodes();
 //         const auto& voltageSources = mnaCircuit.getOrderedVoltageSources();
 //         const auto& inductors = mnaCircuit.getOrderedInductors();
 //         const auto& idealDiodes = mnaCircuit.getOrderedIdealDiodes();
 //         const auto& vcvs_sources = mnaCircuit.getOrderedVCVS();
 //         const auto& ccvs_sources = mnaCircuit.getOrderedCCVS();
 //
 //         int numNonGroundNodes = nonGroundNodes.size();
 //         int numVS = voltageSources.size();
 //         int numL = inductors.size();
 //         int numD = idealDiodes.size();
 //         int numVCVS = vcvs_sources.size();
 //
 //         if (static_cast<size_t>(X.size()) != numNonGroundNodes+numVS+numL+numD+numVCVS+ccvs_sources.size()) {
 //             throw std::runtime_error("Solution vector size does not match number of unknowns.");
 //         }
 //
 //         for (size_t i = 0; i < nonGroundNodes.size(); ++i) nonGroundNodes[i]->setVoltage(X(i));
 //         for (size_t i = 0; i < voltageSources.size(); ++i) voltageSources[i]->setCurrent(X(numNonGroundNodes + i));
 //         for (size_t i = 0; i < inductors.size(); ++i) inductors[i]->setCurrent(X(numNonGroundNodes + numVS + i));
 //         for (size_t i = 0; i < idealDiodes.size(); ++i) idealDiodes[i]->setCurrent(X(numNonGroundNodes + numVS + numL + i));
 //         for (size_t i = 0; i < vcvs_sources.size(); ++i) vcvs_sources[i]->setCurrent(X(numNonGroundNodes + numVS + numL + numD + i));
 //         for (size_t i = 0; i < ccvs_sources.size(); ++i) ccvs_sources[i]->setCurrent(X(numNonGroundNodes + numVS + numL + numD + numVCVS + i));
 //     }
 // };
 //
 // // Forward declaration
 // class centralController;
 //
 // class TransientAnalysis {
 // private:
 //     MakingMNA& mnaCircuit;
 //     MNASolver solver;
 //     centralController& controller; // FIX 5: Reference to controller for checks
 //     double t_step, t_stop;
 // public:
 //     TransientAnalysis(MakingMNA& circuit, centralController& ctrl, double step, double stop)
 //             : mnaCircuit(circuit), solver(), controller(ctrl), t_step(step), t_stop(stop) {
 //         if (t_step <= 0 || t_stop <= 0 || t_step > t_stop) {
 //             throw std::invalid_argument("Invalid time parameters for transient analysis.");
 //         }
 //     }
 //
 //     void run(const vector<string>& output_vars);
 // };
 //
 // class DCSweepAnalysis {
 // private:
 //     MakingMNA& mnaCircuit;
 //     MNASolver solver;
 //     centralController& controller; // FIX 5: Reference to controller for checks
 //     string sweepComponentName;
 //     double startValue, endValue, increment;
 // public:
 //     DCSweepAnalysis(MakingMNA& circuit, centralController& ctrl, const string& compName, double start, double end, double inc)
 //             : mnaCircuit(circuit), solver(), controller(ctrl), sweepComponentName(compName), startValue(start), endValue(end), increment(inc) {
 //         if (increment == 0) throw std::invalid_argument("DC sweep increment cannot be zero.");
 //         if ((endValue > startValue && increment < 0) || (endValue < startValue && increment > 0)) {
 //             increment = -increment;
 //         }
 //     }
 //     void run(const vector<string>& output_vars);
 // };
 //
 // // ============================================================================================
 // // ||                  بخش کنترلر و نمایش (Controller & View)                                 ||
 // // ============================================================================================
 //
 // class CommandParser;
 //
 // bool checkDouble(const string& s);
 // bool checkingNemadElmi(const string& s);
 // pair<string, string> valuate(const string& s);
 // double stringToDouble(const string& s);
 // string trim(const string& str);
 //
 // struct AnalysisParams {
 //     string type;
 //     vector<string> params;
 // };
 //
 // class centralController {
 // private:
 //     MakingMNA circuit_manager;
 //     map<string, Node*> nodes_map;
 //     vector<Element*> elements_list;
 //     map<string, Element*> elements_map;
 //     AnalysisParams last_analysis;
 //     bool is_in_file_menu = false;
 //     vector<string> schematic_files;
 //     string schematics_directory = "./circuits";
 //     string schematics_index_file = schematics_directory + "/schematics_index.txt";
 //
 //     Node* getNode(const string& name) {
 //         if (nodes_map.find(name) == nodes_map.end()) {
 //             nodes_map[name] = new Node(name);
 //         }
 //         return nodes_map[name];
 //     }
 //
 //     // FIX 4: متد برای پاکسازی گره‌های بدون اتصال
 //     void cleanupUnusedNodes() {
 //         if (elements_list.empty()) { // اگر هیچ المانی نیست، همه گره‌ها بجز زمین را حذف کن
 //             vector<string> nodes_to_delete;
 //             for (auto const& [name, node_ptr] : nodes_map) {
 //                 if (!node_ptr->isGround()) {
 //                     nodes_to_delete.push_back(name);
 //                 }
 //             }
 //             for (const auto& name : nodes_to_delete) {
 //                 delete nodes_map[name];
 //                 nodes_map.erase(name);
 //             }
 //             return;
 //         }
 //
 //         set<Node*> used_nodes;
 //         for (const auto& elem : elements_list) {
 //             used_nodes.insert(elem->getNode1());
 //             used_nodes.insert(elem->getNode2());
 //         }
 //
 //         vector<string> nodes_to_delete;
 //         for (auto const& [name, node_ptr] : nodes_map) {
 //             if (used_nodes.find(node_ptr) == used_nodes.end() && !node_ptr->isGround()) {
 //                 nodes_to_delete.push_back(name);
 //             }
 //         }
 //
 //         for (const auto& name : nodes_to_delete) {
 //             cout << "Info: Deleting unused node " << name << "." << endl;
 //             delete nodes_map[name];
 //             nodes_map.erase(name);
 //         }
 //     }
 //
 //     void loadAvailableSchematics() {
 //         schematic_files.clear();
 //         ifstream index_file(schematics_index_file);
 //         if (index_file.is_open()) {
 //             string line;
 //             while (getline(index_file, line)) {
 //                 line = trim(line);
 //                 if (!line.empty()) {
 //                     schematic_files.push_back(line);
 //                 }
 //             }
 //             index_file.close();
 //         } else {
 //             ofstream new_index(schematics_index_file);
 //             if (!new_index.is_open()) {
 //                  cerr << "Warning: Could not access schematics directory '" << schematics_directory << "'. Please ensure it exists." << endl;
 //             } else {
 //                  new_index.close();
 //             }
 //         }
 //         sort(schematic_files.begin(), schematic_files.end());
 //     }
 //
 //
 // public:
 //     centralController() { loadAvailableSchematics(); }
 //     ~centralController() { clearCircuit(); }
 //
 //     // FIX 5: متدهای عمومی برای دسترسی به نقشه گره‌ها و المان‌ها
 //     bool nodeExists(const string& name) const { return nodes_map.count(name) > 0; }
 //     Node* getNodePtr(const string& name) const {
 //         if (nodes_map.count(name)) return nodes_map.at(name);
 //         return nullptr;
 //     }
 //     bool elementExists(const string& name) const { return elements_map.count(name) > 0; }
 //     Element* getElementPtr(const string& name) const {
 //         if (elements_map.count(name)) return elements_map.at(name);
 //         return nullptr;
 //     }
 //
 //
 //     void clearCircuit() {
 //         for (auto const& [name, node_ptr] : nodes_map) delete node_ptr;
 //         for (auto elem_ptr : elements_list) delete elem_ptr;
 //         nodes_map.clear();
 //         elements_list.clear();
 //         elements_map.clear();
 //         cout << "Current circuit cleared." << endl;
 //     }
 //
 //     bool isInFileMenu() const { return is_in_file_menu; }
 //     void setFileMenuState(bool state) { is_in_file_menu = state; }
 //
 //     void handleShowExistingSchematicsCommand() {
 //         is_in_file_menu = true;
 //         loadAvailableSchematics();
 //
 //         cout << "-choose existing schematic:" << endl;
 //         if (schematic_files.empty()) {
 //             cout << " (No schematics found. Use 'NewFile <name>' to save one.)" << endl;
 //         } else {
 //             for (size_t i = 0; i < schematic_files.size(); ++i) {
 //                 cout << i + 1 << "-" << schematic_files[i] << endl;
 //             }
 //         }
 //         cout << " (Enter number to load, 'NewFile <name>' to save current, or 'return' to exit)" << endl;
 //     }
 //
 //     void handleChooseSchematic(const string& choice, CommandParser* parser);
 //
 //     void handleNewFileCommand(const string& filename) {
 //         if (filename.empty() || filename.find_first_of("\\/:*?\"<>|") != string::npos) {
 //             cout << "Error: Invalid filename." << endl;
 //             return;
 //         }
 //
 //         string full_path = schematics_directory + "/" + filename + ".txt";
 //         ofstream file(full_path);
 //         if (!file.is_open()) {
 //             cout << "Error: Could not create file " << full_path << ". Please ensure the 'circuits' directory exists." << endl;
 //             return;
 //         }
 //
 //         for (const auto* elem : elements_list) {
 //              file << elem->getName() << " " << elem->getNode1()->getName() << " " << elem->getNode2()->getName() << " " << elem->getValueAsString() << endl;
 //         }
 //         file.close();
 //
 //         loadAvailableSchematics();
 //         bool found = false;
 //         for (const auto& fname : schematic_files) if (fname == filename) { found = true; break; }
 //
 //         if (!found) {
 //             ofstream index_file(schematics_index_file, ios_base::app);
 //             if (index_file.is_open()) { index_file << filename << endl; index_file.close();
 //             } else { cout << "Warning: Could not update schematics index file." << endl; }
 //         }
 //
 //         cout << "SUCCESS: Current circuit saved to '" << filename << ".txt'." << endl;
 //         if (is_in_file_menu) handleShowExistingSchematicsCommand();
 //     }
 //
 //
 //     void makingResistor(const string& name, const string& n1_name, const string& n2_name, const string& val_str) {
 //         if (elements_map.count(name)) throw logic_error("Error: Element " + name + " already exists in the circuit");
 //         elements_map[name] = new Resistor(getNode(n1_name), getNode(n2_name), name, stringToDouble(val_str));
 //         elements_list.push_back(elements_map[name]);
 //     }
 //
 //     void makingCapacity(const string& name, const string& n1_name, const string& n2_name, const string& val_str) {
 //         if (elements_map.count(name)) throw logic_error("Error: Element " + name + " already exists in the circuit");
 //         elements_map[name] = new Capacitor(getNode(n1_name), getNode(n2_name), name, stringToDouble(val_str));
 //         elements_list.push_back(elements_map[name]);
 //     }
 //
 //     void makingInductor(const string& name, const string& n1_name, const string& n2_name, const string& val_str) {
 //         if (elements_map.count(name)) throw logic_error("Error: Element " + name + " already exists in the circuit");
 //         elements_map[name] = new Inductor(getNode(n1_name), getNode(n2_name), name, stringToDouble(val_str));
 //         elements_list.push_back(elements_map[name]);
 //     }
 //
 //     void makingDiode(const string& name, const string& n1_name, const string& n2_name, const string& model) {
 //         if (elements_map.count(name)) throw logic_error("Error: Element " + name + " already exists in the circuit");
 //         elements_map[name] = new IdealDiode(getNode(n1_name), getNode(n2_name), name, model);
 //         elements_list.push_back(elements_map[name]);
 //     }
 //
 //     void makingVoltageSourceDC(const string& name, const string& n_plus, const string& n_minus, const string& val_str) {
 //         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
 //         elements_map[name] = new VoltageSource(getNode(n_plus), getNode(n_minus), name, stringToDouble(val_str));
 //         elements_list.push_back(elements_map[name]);
 //     }
 //
 //     void makingCurrentSourceDC(const string& name, const string& n_plus, const string& n_minus, const string& val_str) {
 //         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
 //         elements_map[name] = new CurrentSource(getNode(n_plus), getNode(n_minus), name, stringToDouble(val_str));
 //         elements_list.push_back(elements_map[name]);
 //     }
 //
 //     void makingVoltageSourceSin(const string& name, const string& n_plus, const string& n_minus, const string& v_off, const string& v_amp, const string& freq) {
 //          if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
 //         auto* vs = new VoltageSource(getNode(n_plus), getNode(n_minus), name, 0.0);
 //         vs->setSinParams(stringToDouble(v_off), stringToDouble(v_amp), stringToDouble(freq));
 //         elements_map[name] = vs;
 //         elements_list.push_back(elements_map[name]);
 //     }
 //
 //     // FIX 1: پیاده‌سازی منبع جریان سینوسی
 //     void makingCurrentSourceSin(const string& name, const string& n_plus, const string& n_minus, const string& i_off, const string& i_amp, const string& freq) {
 //         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
 //         auto* cs = new CurrentSource(getNode(n_plus), getNode(n_minus), name, 0.0);
 //         cs->setSinParams(stringToDouble(i_off), stringToDouble(i_amp), stringToDouble(freq));
 //         elements_map[name] = cs;
 //         elements_list.push_back(elements_map[name]);
 //     }
 //
 //     void makingVoltageSourcePulse(const string& name, const string& n_plus, const string& n_minus,
 //                                   const string& v1, const string& v2, const string& td,
 //                                   const string& tr, const string& tf, const string& pw,
 //                                   const string& period) {
 //         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
 //         auto* vs = new VoltageSource(getNode(n_plus), getNode(n_minus), name, 0.0);
 //         vs->setPulseParams(stringToDouble(v1), stringToDouble(v2), stringToDouble(td), stringToDouble(tr), stringToDouble(tf), stringToDouble(pw), stringToDouble(period));
 //         elements_map[name] = vs;
 //         elements_list.push_back(elements_map[name]);
 //     }
 //
 //     // FIX 1: پیاده‌سازی منبع جریان پالسی
 //     void makingCurrentSourcePulse(const string& name, const string& n_plus, const string& n_minus,
 //                                   const string& i1, const string& i2, const string& td,
 //                                   const string& tr, const string& tf, const string& pw,
 //                                   const string& period) {
 //         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
 //         auto* cs = new CurrentSource(getNode(n_plus), getNode(n_minus), name, 0.0);
 //         cs->setPulseParams(stringToDouble(i1), stringToDouble(i2), stringToDouble(td), stringToDouble(tr), stringToDouble(tf), stringToDouble(pw), stringToDouble(period));
 //         elements_map[name] = cs;
 //         elements_list.push_back(elements_map[name]);
 //     }
 //
 //     void makingVCVS(const string& name, const string& n_p, const string& n_m, const string& nc_p, const string& nc_m, const string& gain_str) {
 //         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
 //         elements_map[name] = new VCVS(getNode(n_p), getNode(n_m), name, getNode(nc_p), getNode(nc_m), stringToDouble(gain_str));
 //         elements_list.push_back(elements_map[name]);
 //     }
 //
 //     void makingVCCS(const string& name, const string& n_p, const string& n_m, const string& nc_p, const string& nc_m, const string& gain_str) {
 //          if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
 //         elements_map[name] = new VCCS(getNode(n_p), getNode(n_m), name, getNode(nc_p), getNode(nc_m), stringToDouble(gain_str));
 //         elements_list.push_back(elements_map[name]);
 //     }
 //
 //     void makingCCVS(const string& name, const string& n_p, const string& n_m, const string& vctrl_name, const string& gain_str) {
 //         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
 //         if (!elements_map.count(vctrl_name) || dynamic_cast<VoltageSource*>(elements_map[vctrl_name]) == nullptr) {
 //             throw logic_error("Error: Control voltage source " + vctrl_name + " not found for CCVS " + name);
 //         }
 //         elements_map[name] = new CCVS(getNode(n_p), getNode(n_m), name, vctrl_name, stringToDouble(gain_str));
 //         elements_list.push_back(elements_map[name]);
 //     }
 //
 //     void makingCCCS(const string& name, const string& n_p, const string& n_m, const string& vctrl_name, const string& gain_str) {
 //         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
 //         if (!elements_map.count(vctrl_name) || dynamic_cast<VoltageSource*>(elements_map[vctrl_name]) == nullptr) {
 //             throw logic_error("Error: Control voltage source " + vctrl_name + " not found for CCCS " + name);
 //         }
 //         elements_map[name] = new CCCS(getNode(n_p), getNode(n_m), name, vctrl_name, stringToDouble(gain_str));
 //         elements_list.push_back(elements_map[name]);
 //     }
 //
 //     void deletingElement(const string& name_to_delete) {
 //         if (!elements_map.count(name_to_delete)) throw logic_error("Error: Cannot delete component; component '" + name_to_delete + "' not found");
 //
 //         // FIX 3: بررسی اینکه آیا المان، کنترل‌کننده منبع وابسته است یا خیر
 //         for (const auto& elem : elements_list) {
 //             if (auto ccvs = dynamic_cast<CCVS*>(elem)) {
 //                 if (ccvs->getControlVoltageSourceName() == name_to_delete) {
 //                     throw logic_error("Error: Cannot delete component '" + name_to_delete + "' because it is a control branch for source '" + ccvs->getName() + "'.");
 //                 }
 //             } else if (auto cccs = dynamic_cast<CCCS*>(elem)) {
 //                  if (cccs->getControlVoltageSourceName() == name_to_delete) {
 //                     throw logic_error("Error: Cannot delete component '" + name_to_delete + "' because it is a control branch for source '" + cccs->getName() + "'.");
 //                 }
 //             }
 //         }
 //
 //         Element* elem_to_delete_ptr = elements_map[name_to_delete];
 //         elements_map.erase(name_to_delete);
 //         elements_list.erase(remove(elements_list.begin(), elements_list.end(), elem_to_delete_ptr), elements_list.end());
 //
 //         delete elem_to_delete_ptr;
 //         cout << "SUCCESS: Component " << name_to_delete << " deleted." << endl;
 //
 //         // FIX 4: فراخوانی پاکسازی گره‌های استفاده نشده
 //         cleanupUnusedNodes();
 //     }
 //
 //     void addingGround(const string& node_name){
 //         getNode(node_name);
 //         cout << "SUCCESS: Node " << node_name << " is designated as a ground reference." << endl;
 //     }
 //
 //     void handleNodesCommand() {
 //         if (nodes_map.empty()) { cout << "No nodes in the circuit yet." << endl; return; }
 //         cout << "Available nodes:" << endl;
 //         for(auto const& [name, node_ptr] : nodes_map) { cout << name << " "; }
 //         cout << endl;
 //     }
 //
 //     void handleListCommand(const string& componentType = "") {
 //         if (elements_list.empty()) { cout << "No elements in the circuit yet." << endl; return; }
 //         bool found = false;
 //         cout << "Circuit elements:" << endl;
 //         for(auto const& elem : elements_list) {
 //             bool type_match = false;
 //             string elem_type_prefix = elem->getName().substr(0,1);
 //             if (componentType.empty()) type_match = true;
 //             else if (componentType == elem_type_prefix) type_match = true;
 //
 //             if (type_match) {
 //                 cout << "- " << elem->getName() << " connected between " << elem->getNode1()->getName() << " and " << elem->getNode2()->getName() << endl;
 //                 found = true;
 //             }
 //         }
 //         if (!found) cout << "No elements of type '" << componentType << "' found." << endl;
 //     }
 //
 //     void handleRenameNodeCommand(const string& oldName, const string& newName) {
 //         if (nodes_map.find(oldName) == nodes_map.end()) {
 //             throw logic_error("ERROR: Node " + oldName + " does not exist in the circuit");
 //         }
 //         if (nodes_map.find(newName) != nodes_map.end()) {
 //             throw logic_error("ERROR: Node name " + newName + " already exists");
 //         }
 //         Node* node_ptr = nodes_map[oldName];
 //         node_ptr->setName(newName); // FIX 6: نام داخلی شیء گره نیز تغییر می‌کند
 //         nodes_map.erase(oldName);
 //         nodes_map[newName] = node_ptr;
 //         cout << "SUCCESS: Node renamed from " << oldName << " to " << newName << endl;
 //     }
 //
 //     void defineAnalysis(const string& type, const vector<string>& params) {
 //         last_analysis.type = type;
 //         last_analysis.params = params;
 //         cout << "SUCCESS: " << type << " analysis defined." << endl;
 //     }
 //
 //     void handlePrintCommand(const string& analysisType,
 //                             const vector<string>& analysisParams,
 //                             const string& outputVarsStr) {
 //
 //         vector<string> output_vars;
 //         stringstream ss(outputVarsStr);
 //         string temp;
 //         while (ss >> temp) { output_vars.push_back(temp); }
 //
 //         if (output_vars.empty()){ throw logic_error("Error: Missing output variables for print command."); }
 //
 //         const AnalysisParams* analysis_to_run = nullptr;
 //         AnalysisParams temp_analysis;
 //
 //         if (!analysisParams.empty()){
 //             temp_analysis.type = analysisType; temp_analysis.params = analysisParams;
 //             analysis_to_run = &temp_analysis;
 //         } else if (last_analysis.type == analysisType) {
 //             analysis_to_run = &last_analysis;
 //         } else {
 //             throw logic_error("Error: No analysis parameters provided or defined for " + analysisType);
 //         }
 //
 //         vector<Node*> nodes_vector;
 //         for(auto const& [name, ptr] : nodes_map) nodes_vector.push_back(ptr);
 //         circuit_manager.setCircuitData(nodes_vector, elements_list);
 //
 //
 //         if (analysis_to_run->type == "TRAN") {
 //             if (analysis_to_run->params.size() < 2) throw logic_error("Error: Not enough parameters for TRAN analysis.");
 //             double tstep = stringToDouble(analysis_to_run->params[0]);
 //             double tstop = stringToDouble(analysis_to_run->params[1]);
 //             TransientAnalysis tran(circuit_manager, *this, tstep, tstop);
 //             tran.run(output_vars);
 //
 //         } else if (analysis_to_run->type == "DC") {
 //             if (analysis_to_run->params.size() < 4) throw logic_error("Error: Not enough parameters for DC analysis.");
 //             string srcName = analysis_to_run->params[0];
 //             double start = stringToDouble(analysis_to_run->params[1]);
 //             double end = stringToDouble(analysis_to_run->params[2]);
 //             double inc = stringToDouble(analysis_to_run->params[3]);
 //             DCSweepAnalysis dc(circuit_manager, *this, srcName, start, end, inc);
 //             dc.run(output_vars);
 //         }
 //     }
 // };
 //
 // // ============================================================================================
 // // ||                  بخش پردازشگر دستورات (View)                                             ||
 // // ============================================================================================
 //
 // string trim(const string& str) {
 //     const string whitespace = " \t\n\r\f\v";
 //     size_t start = str.find_first_not_of(whitespace);
 //     if (string::npos == start) return "";
 //     size_t end = str.find_last_not_of(whitespace);
 //     return str.substr(start, end - start + 1);
 // }
 //
 // bool checkDouble(const string& s_in) {
 //     if (s_in.empty()) return false;
 //     string s = s_in; size_t i = 0;
 //     if (s[0] == '-') { if (s.length() == 1) return false; i = 1; }
 //     bool digitFound = false; bool dotFound = false;
 //     for (; i < s.length(); ++i) {
 //         if (isdigit(s[i])) { digitFound = true; }
 //         else if (s[i] == '.') { if (dotFound) return false; dotFound = true; }
 //         else { return false; }
 //     }
 //     if (!digitFound) return false;
 //     try { stod(s); } catch (const std::invalid_argument&) { return false; } catch (const std::out_of_range&) { return false; }
 //     return true;
 // }
 //
 // bool checkingNemadElmi(const string& s_in) {
 //     if (s_in.empty()) return false;
 //     string s = s_in; std::transform(s.begin(), s.end(), s.begin(), ::tolower);
 //     size_t e_pos = s.find('e');
 //     if (e_pos == string::npos || e_pos == 0 || e_pos == s.length() - 1) return false;
 //     string base_str = s.substr(0, e_pos); string exp_str = s.substr(e_pos + 1);
 //     if (base_str.empty() || exp_str.empty()) return false; if (!checkDouble(base_str)) return false;
 //     size_t i = 0; if (exp_str[0] == '+' || exp_str[0] == '-') { if (exp_str.length() == 1) return false; i = 1; }
 //     if (i == exp_str.length()) return false;
 //     for (; i < exp_str.length(); ++i) { if (!isdigit(exp_str[i])) return false; }
 //     try { stod(s_in); } catch (const std::out_of_range&) { return false; }
 //     return true;
 // }
 //
 // pair<string, string> valuate(const string& s_in) {
 //     string s = s_in; string value_str = s; string prefix_str = "";
 //     if (s.empty()) return {s, ""};
 //     if (s.length() >= 3 && (s.substr(s.length() - 3) == "Meg" || s.substr(s.length() - 3) == "MEG")) {
 //         value_str = s.substr(0, s.length() - 3); prefix_str = "Meg";
 //     } else if (s.length() >= 2) {
 //         char last_char = s.back(); string potential_val_str = s.substr(0, s.length() - 1);
 //         if (!potential_val_str.empty() && (isdigit(potential_val_str.back()) || potential_val_str.back() == '.' || (potential_val_str.length() == 1 && potential_val_str[0] == '-'))) {
 //             switch (last_char) {
 //                 case 'G': case 'g': prefix_str = "G"; value_str = potential_val_str; break;
 //                 case 'k': case 'K': prefix_str = "k"; value_str = potential_val_str; break;
 //                 case 'm': prefix_str = "m"; value_str = potential_val_str; break;
 //                 case 'u': case 'U': prefix_str = "u"; value_str = potential_val_str; break;
 //                 case 'n': case 'N': prefix_str = "n"; value_str = potential_val_str; break;
 //                 case 'p': case 'P': prefix_str = "p"; value_str = potential_val_str; break;
 //                 case 'f': case 'F': prefix_str = "f"; value_str = potential_val_str; break;
 //             }
 //         }
 //     }
 //     if (value_str.empty() && !prefix_str.empty()) { return {s_in, ""}; }
 //     return {value_str, prefix_str};
 // }
 //
 // double stringToDouble(const string& s_in) {
 //     pair<string, string> p_val = valuate(s_in);
 //     double base_val = stod(p_val.first);
 //     string prefix = p_val.second;
 //
 //     if (prefix == "G" || prefix == "g") return base_val * 1e9;
 //     if (prefix == "Meg") return base_val * 1e6;
 //     if (prefix == "k" || prefix == "K") return base_val * 1e3;
 //     if (prefix == "m") return base_val * 1e-3;
 //     if (prefix == "u" || prefix == "U") return base_val * 1e-6;
 //     if (prefix == "n" || prefix == "N") return base_val * 1e-9;
 //     if (prefix == "p" || prefix == "P") return base_val * 1e-12;
 //     if (prefix == "f" || prefix == "F") return base_val * 1e-15;
 //
 //     return base_val;
 // }
 //
 // class CommandParser {
 // private:
 //     centralController& controller;
 //
 //     void validateParameter(const string& param_name, const string& param_val_raw, const string& context_name, bool allow_zero, bool must_be_positive, bool must_be_non_negative) {
 //         pair<string, string> p_param = valuate(param_val_raw);
 //         if (!checkDouble(p_param.first) && !checkingNemadElmi(p_param.first)) {
 //             throw logic_error("Error: Invalid format for " + param_name + " in " + context_name + " (" + param_val_raw + ")");
 //         }
 //         double val = stod(p_param.first);
 //         if (must_be_positive && val <= 0) {
 //             throw logic_error("Error: Parameter " + param_name + " must be positive in " + context_name + " (" + param_val_raw + ")");
 //         }
 //         if (must_be_non_negative && val < 0) {
 //              throw logic_error("Error: Parameter " + param_name + " cannot be negative in " + context_name + " (" + param_val_raw + ")");
 //         }
 //         if (!allow_zero && val == 0) {
 //              throw logic_error("Error: Parameter " + param_name + " cannot be zero in " + context_name + " (" + param_val_raw + ")");
 //         }
 //     }
 //
 //     // Parses SPICE-like netlist lines (e.g., from a file)
 //     bool tryParseNetlistLine(const string& in) {
 //         smatch matches;
 //         regex netlist_pattern(R"(^([a-zA-Z]\S*)\s+(\S+)\s+(\S+)\s+(.+)$)");
 //         if (regex_match(in, matches, netlist_pattern)) {
 //             string command = "add " + in;
 //             try {
 //                 if (tryParseElementCommands(command) || tryParseSourceCommands(command)) { return true; }
 //             } catch (const exception& e) {
 //                  cerr << "Error processing line from file: '" << in << "'. Reason: " << e.what() << endl;
 //             }
 //         }
 //         return false;
 //     }
 //
 //
 //     bool tryParseElementCommands(const string& in) {
 //         smatch matches;
 //         regex add_res_pattern(R"(^add\s+(R\S*)\s+(\S+)\s+(\S+)\s+(.+)$)");
 //         if (regex_match(in, matches, add_res_pattern)) {
 //             validateParameter("Resistance", matches[4].str(), "Resistor " + matches[1].str(), false, true, false);
 //             controller.makingResistor(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
 //         }
 //
 //         regex add_cap_pattern(R"(^add\s+(C\S*)\s+(\S+)\s+(\S+)\s+(.+)$)");
 //         if (regex_match(in, matches, add_cap_pattern)) {
 //             validateParameter("Capacitance", matches[4].str(), "Capacitor " + matches[1].str(), false, true, false);
 //             controller.makingCapacity(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
 //         }
 //
 //         regex add_ind_pattern(R"(^add\s+(L\S*)\s+(\S+)\s+(\S+)\s+(.+)$)");
 //         if (regex_match(in, matches, add_ind_pattern)) {
 //             validateParameter("Inductance", matches[4].str(), "Inductor " + matches[1].str(), false, true, false);
 //             controller.makingInductor(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
 //         }
 //
 //         regex add_diode_pattern(R"(^add\s+(D\S*)\s+(\S+)\s+(\S+)\s+(D|Z)$)");
 //         if (regex_match(in, matches, add_diode_pattern)) {
 //             controller.makingDiode(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
 //         }
 //
 //         regex add_gnd_pattern(R"(^add\s+GND\s+(\S+)$)");
 //         if (regex_match(in, matches, add_gnd_pattern)) {
 //             controller.addingGround(matches[1].str()); return true;
 //         }
 //
 //         regex del_elem_pattern(R"(^delete\s+([RCLD]\S*)$)");
 //         if(regex_match(in, matches, del_elem_pattern)) {
 //             controller.deletingElement(matches[1].str()); return true;
 //         }
 //
 //         return false;
 //     }
 //
 //     bool tryParseSourceCommands(const string& in) {
 //         smatch matches;
 //         // Regex for V and I sources
 //         regex add_src_generic_pattern(R"(^add\s+([VI]\S*)\s+(\S+)\s+(\S+)\s+(.+)$)");
 //         if (regex_match(in, matches, add_src_generic_pattern)) {
 //             string name = matches[1].str();
 //             char type = name[0];
 //             string n1 = matches[2].str();
 //             string n2 = matches[3].str();
 //             string val_full_str = trim(matches[4].str());
 //
 //             // FIX: Updated Regex to handle both comma and space separators for SIN
 //             regex sin_params_pattern(R"(^SIN\s*\(\s*(\S+?)[,\s]+(\S+?)[,\s]+(\S+?)\s*\)$)");
 //             smatch sin_matches;
 //             if (regex_match(val_full_str, sin_matches, sin_params_pattern)) {
 //                 validateParameter("Frequency", sin_matches[3].str(), "SIN source " + name, false, true, false);
 //                 if (type == 'V') controller.makingVoltageSourceSin(name, n1, n2, sin_matches[1].str(), sin_matches[2].str(), sin_matches[3].str());
 //                 else controller.makingCurrentSourceSin(name, n1, n2, sin_matches[1].str(), sin_matches[2].str(), sin_matches[3].str());
 //                 return true;
 //             }
 //
 //             regex pulse_params_pattern(R"(^PULSE\s*\(\s*(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s*\)$)");
 //             smatch pulse_matches;
 //             if (regex_match(val_full_str, pulse_matches, pulse_params_pattern)) {
 //                 // FIX 2: اعتبارسنجی دقیق پارامترهای پالس
 //                 validateParameter("Td", pulse_matches[3].str(), "PULSE source " + name, true, false, true);
 //                 validateParameter("Tr", pulse_matches[4].str(), "PULSE source " + name, false, true, false);
 //                 validateParameter("Tf", pulse_matches[5].str(), "PULSE source " + name, false, true, false);
 //                 validateParameter("PW", pulse_matches[6].str(), "PULSE source " + name, true, false, true);
 //                 validateParameter("Period", pulse_matches[7].str(), "PULSE source " + name, false, true, false);
 //                 if(stringToDouble(pulse_matches[4].str()) + stringToDouble(pulse_matches[5].str()) > stringToDouble(pulse_matches[7].str())){
 //                     throw logic_error("Error: Sum of rise and fall times must be less than period for PULSE source " + name);
 //                 }
 //
 //                 if (type == 'V') controller.makingVoltageSourcePulse(name, n1, n2, pulse_matches[1].str(), pulse_matches[2].str(), pulse_matches[3].str(), pulse_matches[4].str(), pulse_matches[5].str(), pulse_matches[6].str(), pulse_matches[7].str());
 //                 else controller.makingCurrentSourcePulse(name, n1, n2, pulse_matches[1].str(), pulse_matches[2].str(), pulse_matches[3].str(), pulse_matches[4].str(), pulse_matches[5].str(), pulse_matches[6].str(), pulse_matches[7].str());
 //                 return true;
 //             }
 //
 //             validateParameter("Value", val_full_str, "DC source " + name, true, false, false);
 //             if(type == 'V') controller.makingVoltageSourceDC(name, n1, n2, val_full_str);
 //             else controller.makingCurrentSourceDC(name, n1, n2, val_full_str);
 //             return true;
 //         }
 //
 //         // Dependent Sources
 //         regex add_vcvs_pattern(R"(^add\s+(E\S*)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(.+)$)");
 //         if (regex_match(in, matches, add_vcvs_pattern)) {
 //              controller.makingVCVS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str(), matches[6].str()); return true;
 //         }
 //         regex add_vccs_pattern(R"(^add\s+(G\S*)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(.+)$)");
 //         if (regex_match(in, matches, add_vccs_pattern)) {
 //             controller.makingVCCS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str(), matches[6].str()); return true;
 //         }
 //         regex add_ccvs_pattern(R"(^add\s+(H\S*)\s+(\S+)\s+(\S+)\s+(V\S+)\s+(.+)$)");
 //         if (regex_match(in, matches, add_ccvs_pattern)) {
 //             controller.makingCCVS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str()); return true;
 //         }
 //         regex add_cccs_pattern(R"(^add\s+(F\S*)\s+(\S+)\s+(\S+)\s+(V\S+)\s+(.+)$)");
 //         if (regex_match(in, matches, add_cccs_pattern)) {
 //             controller.makingCCCS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str()); return true;
 //         }
 //         regex del_src_pattern(R"(^delete\s+([VIEGHF]\S*)$)");
 //         if(regex_match(in, matches, del_src_pattern)) {
 //             controller.deletingElement(matches[1].str()); return true;
 //         }
 //         return false;
 //     }
 //
 //     bool tryParseAnalysisDefinitionCommands(const string& in) {
 //         smatch matches;
 //         regex tran_def_pattern(R"(^\.TRAN\s+(\S+)\s+(\S+)(?:\s+(\S+))?(?:\s+(\S+))?\s*$)");
 //         if (regex_match(in, matches, tran_def_pattern)) {
 //             vector<string> params = {matches[1].str(), matches[2].str()};
 //             if(matches[3].matched) params.push_back(matches[3].str());
 //             if(matches[4].matched) params.push_back(matches[4].str());
 //             controller.defineAnalysis("TRAN", params); return true;
 //         }
 //
 //         regex dc_def_pattern(R"(^\.DC\s+([VI]\w*)\s+(\S+)\s+(\S+)\s+(\S+)\s*$)");
 //         if (regex_match(in, matches, dc_def_pattern)) {
 //             vector<string> params = {matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()};
 //             controller.defineAnalysis("DC", params); return true;
 //         }
 //         return false;
 //     }
 //
 //     bool tryParseNodeCommands(const string& in) {
 //         if (in == "nodes") { controller.handleNodesCommand(); return true; }
 //
 //         smatch matches;
 //         regex rename_node_full_pattern(R"(^rename\s+node\s+(\S+)\s+(\S+)$)");
 //         if (regex_match(in, matches, rename_node_full_pattern)) {
 //             controller.handleRenameNodeCommand(matches[1].str(), matches[2].str()); return true;
 //         } else if (in.rfind("rename node", 0) == 0) {
 //             throw logic_error("ERROR: Invalid syntax correct format: rename node <old_name> <new_name>");
 //         }
 //         return false;
 //     }
 //
 //     bool tryParseListCommands(const string& in) {
 //         if (in == "list") { controller.handleListCommand(); return true; }
 //
 //         smatch matches;
 //         regex list_comp_pattern(R"(^list\s+([RCLDVIEGHF])$)");
 //         if (regex_match(in, matches, list_comp_pattern)) {
 //             controller.handleListCommand(matches[1].str()); return true;
 //         }
 //         return false;
 //     }
 //
 //     bool tryParsePrintCommands(const string& in) {
 //         smatch matches;
 //         regex print_base_pattern(R"(^print\s+(TRAN|DC|AC)\s*(.*)$)");
 //         if (regex_match(in, matches, print_base_pattern)) {
 //             string analysisType = matches[1].str();
 //             string remaining_args = trim(matches[2].str());
 //             vector<string> analysisParams;
 //             string outputVarsStr;
 //
 //             stringstream ss(remaining_args);
 //             string word;
 //             vector<string> words;
 //             while(ss >> word) words.push_back(word);
 //
 //             size_t var_start_index = 0;
 //             if (analysisType == "TRAN" && words.size() >= 2) {
 //                 if( (checkDouble(valuate(words[0]).first) || checkingNemadElmi(valuate(words[0]).first)) &&
 //                     (checkDouble(valuate(words[1]).first) || checkingNemadElmi(valuate(words[1]).first)) ){
 //                     analysisParams.push_back(words[0]);
 //                     analysisParams.push_back(words[1]);
 //                     var_start_index = 2;
 //                 }
 //             } else if (analysisType == "DC" && words.size() >= 4) {
 //                  if( (checkDouble(valuate(words[1]).first) || checkingNemadElmi(valuate(words[1]).first)) ){
 //                     analysisParams = {words[0], words[1], words[2], words[3]};
 //                     var_start_index = 4;
 //                  }
 //             }
 //
 //             stringstream vars_ss;
 //             for(size_t i = var_start_index; i < words.size(); ++i) { vars_ss << words[i] << " "; }
 //             outputVarsStr = vars_ss.str();
 //
 //             controller.handlePrintCommand(analysisType, analysisParams, outputVarsStr);
 //             return true;
 //         }
 //         return false;
 //     }
 //
 //     bool tryParseFileAndMenuCommands(const string& in) {
 //         if (in == "show existing schematics") { controller.handleShowExistingSchematicsCommand(); return true; }
 //         smatch matches;
 //         regex new_file_pattern(R"(^NewFile\s+(\S+)$)");
 //         if (regex_match(in, matches, new_file_pattern)) { controller.handleNewFileCommand(matches[1].str()); return true; }
 //         if(controller.isInFileMenu()){ controller.handleChooseSchematic(in, this); return true; }
 //         return false;
 //     }
 //
 //
 // public:
 //     explicit CommandParser(centralController& ctrl) : controller(ctrl) {}
 //
 //     void processInput(const string& in) {
 //         string trimmed_in = trim(in);
 //         if (trimmed_in.empty()) return;
 //
 //         if (tryParseFileAndMenuCommands(trimmed_in)) return;
 //         if (tryParseAnalysisDefinitionCommands(trimmed_in)) return;
 //         if (tryParseNodeCommands(trimmed_in)) return;
 //         if (tryParseListCommands(trimmed_in)) return;
 //         if (tryParsePrintCommands(trimmed_in)) return;
 //         if (tryParseElementCommands(trimmed_in)) return;
 //         if (tryParseSourceCommands(trimmed_in)) return;
 //         if (tryParseNetlistLine(trimmed_in)) return; // For processing file lines
 //
 //         throw logic_error("Error: Unknown command or invalid syntax. -> " + in);
 //     }
 // };
 //
 // void centralController::handleChooseSchematic(const string& choice, CommandParser* parser) {
 //     if (choice == "return") {
 //         is_in_file_menu = false;
 //         cout << "Returning to main menu..." << endl;
 //         return;
 //     }
 //
 //     try {
 //         size_t choice_num = stoul(choice);
 //         if (choice_num > 0 && choice_num <= schematic_files.size()) {
 //             string filename = schematic_files[choice_num - 1];
 //             string full_path = schematics_directory + "/" + filename + ".txt";
 //
 //             ifstream file(full_path);
 //             if (!file.is_open()) { throw logic_error("Error: Could not open file " + filename); }
 //
 //             clearCircuit();
 //             cout << "Loading schematic '" << filename << "'..." << endl;
 //             cout << filename << ".txt:" << endl;
 //
 //             setFileMenuState(false); // Temporarily exit file menu mode
 //
 //             string line;
 //             while(getline(file, line)) {
 //                 line = trim(line);
 //                 if (line.empty() || line[0] == '*' || line[0] == '.') continue;
 //                 cout << line << endl;
 //                 try {
 //                     parser->processInput(line);
 //                 } catch (const exception& e) {
 //                     cerr << "Error processing line from file: '" << line << "' -> " << e.what() << endl;
 //                 }
 //             }
 //             file.close();
 //             setFileMenuState(true); // Re-enter file menu mode
 //
 //         } else {
 //             cout << "-Error: Inappropriate input" << endl;
 //         }
 //     } catch (const exception& e) {
 //         cout << "-Error: Inappropriate input" << endl;
 //     }
 //
 //     handleShowExistingSchematicsCommand();
 // }
 //
 // void TransientAnalysis::run(const vector<string>& output_vars) {
 //     // FIX 5: بررسی وجود گره‌ها و المان‌ها قبل از شروع تحلیل
 //     for (const auto& var : output_vars) {
 //         if (var[0] == 'V' && var[1] == '(') {
 //             string node_name = var.substr(2, var.length() - 3);
 //             if (!controller.nodeExists(node_name)) {
 //                 throw logic_error("Error: Node '" + node_name + "' not found in circuit for TRAN analysis.");
 //             }
 //         } else if (var[0] == 'I' && var[1] == '(') {
 //             string elem_name = var.substr(2, var.length() - 3);
 //             if (!controller.elementExists(elem_name)) {
 //                 throw logic_error("Error: Component '" + elem_name + "' not found in circuit for TRAN analysis.");
 //             }
 //         }
 //     }
 //
 //     double current_time = 0.0;
 //     cout << "Time(s)\t";
 //     for (const auto& var : output_vars) cout << var << "\t";
 //     cout << endl << string(output_vars.size() * 10 + 10, '-') << endl;
 //
 //     mnaCircuit.setTimeStep(t_step);
 //
 //     while (current_time <= t_stop + 1e-9) { // Add tolerance for floating point comparison
 //         Eigen::MatrixXd A = mnaCircuit.getSystemMatrixA(false);
 //         Eigen::VectorXd Z = mnaCircuit.getSystemVectorZ(false, current_time);
 //         Eigen::VectorXd X = solver.solve(A, Z);
 //         solver.updateCircuitState(X, mnaCircuit);
 //
 //         cout << fixed << setprecision(6) << current_time << "\t";
 //         for (const auto& var : output_vars) {
 //              if (var[0] == 'V' && var[1] == '(') {
 //                 string node_name = var.substr(2, var.length() - 3);
 //                 cout << controller.getNodePtr(node_name)->getVoltage() << "\t";
 //             } else if (var[0] == 'I' && var[1] == '(') {
 //                 string elem_name = var.substr(2, var.length() - 3);
 //                 cout << controller.getElementPtr(elem_name)->getCurrent() << "\t";
 //             }
 //         }
 //         cout << endl;
 //
 //         current_time += t_step;
 //
 //         for (auto* node : mnaCircuit.getAllNodesInCircuit()) node->updateVoltageForNextStep();
 //         for (auto* ind : mnaCircuit.getOrderedInductors()) ind->updateCurrentForNextStep();
 //     }
 // }
 //
 // void DCSweepAnalysis::run(const vector<string>& output_vars) {
 //     // FIX 5: بررسی وجود گره‌ها و المان‌ها قبل از شروع تحلیل
 //     for (const auto& var : output_vars) {
 //         if (var[0] == 'V' && var[1] == '(') {
 //             string node_name = var.substr(2, var.length() - 3);
 //             if (!controller.nodeExists(node_name)) {
 //                 throw logic_error("Error: Node '" + node_name + "' not found in circuit for DC analysis.");
 //             }
 //         } else if (var[0] == 'I' && var[1] == '(') {
 //             string elem_name = var.substr(2, var.length() - 3);
 //             if (!controller.elementExists(elem_name)) {
 //                 throw logic_error("Error: Component '" + elem_name + "' not found in circuit for DC analysis.");
 //             }
 //         }
 //     }
 //
 //     Element* sweepElement = controller.getElementPtr(sweepComponentName);
 //     if (!sweepElement) throw std::runtime_error("Sweep component '" + sweepComponentName + "' not found.");
 //     auto* sweepVoltageSource = dynamic_cast<VoltageSource*>(sweepElement);
 //     if (!sweepVoltageSource) throw std::runtime_error("Sweep component must be a VoltageSource.");
 //
 //     cout << sweepComponentName << "\t";
 //     for (const auto& var : output_vars) cout << var << "\t";
 //     cout << endl << string(output_vars.size() * 10 + 10, '-') << endl;
 //
 //     for (double val = startValue; (increment > 0 ? val <= endValue + 1e-9 : val >= endValue - 1e-9) ; val += increment) {
 //         sweepVoltageSource->setValue(val);
 //
 //         Eigen::MatrixXd A = mnaCircuit.getSystemMatrixA(true);
 //         Eigen::VectorXd Z = mnaCircuit.getSystemVectorZ(true);
 //         Eigen::VectorXd X = solver.solve(A, Z);
 //         solver.updateCircuitState(X, mnaCircuit);
 //
 //         cout << fixed << setprecision(6) << val << "\t";
 //         for (const auto& var : output_vars) {
 //              if (var[0] == 'V' && var[1] == '(') {
 //                 string node_name = var.substr(2, var.length() - 3);
 //                 cout << controller.getNodePtr(node_name)->getVoltage() << "\t";
 //             } else if (var[0] == 'I' && var[1] == '(') {
 //                 string elem_name = var.substr(2, var.length() - 3);
 //                 cout << controller.getElementPtr(elem_name)->getCurrent() << "\t";
 //             }
 //         }
 //         cout << endl;
 //     }
 // }
 //
 // // ============================================================================================
 // // ||                  تابع اصلی (Main Function)                                              ||
 // // ============================================================================================
 // int main() {
 //     centralController controller_instance;
 //     CommandParser parser(controller_instance);
 //
 //     cout << "Welcome to Circuit Simulator!" << endl;
 //     cout << "Use 'show existing schematics' to manage files." << endl;
 //     cout << "Please ensure the 'circuits' directory exists in the same folder as the executable." << endl;
 //     cout << "Enter commands or 'exit' to quit." << endl;
 //
 //     string line;
 //     while (cout << ">>> " && getline(cin, line) && line != "exit") {
 //         try {
 //             parser.processInput(line);
 //         } catch (const exception& e) {
 //             cerr << e.what() << endl;
 //         }
 //     }
 //
 //     cout << "Exiting simulator. Goodbye!" << endl;
 //     return 0;
 // }
 //

///////////////////////////////////

//checking full
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <map>
#include <cmath>
#include <regex>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <set>

// Eigen library for matrix operations
// Ensure Eigen headers are in your project path.
#include "Eigen/Dense"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

// ==========================================================================================
// ||                                    بخش مدل (Model)                                   ||
// ==========================================================================================

class Node {
private:
    double voltage;
    string name;
    double previousVoltage;

public:
    Node(const string &name, double voltage = 0.0, double prev_voltage = 0.0)
            : voltage(voltage), name(name), previousVoltage(prev_voltage) {}

    string getName() const { return name; }
    void setName(const string& newName) { this->name = newName; }
    double getVoltage() const { return voltage; }
    void setVoltage(double v) { voltage = v; }
    double getPreviousVoltage() const { return previousVoltage; }
    void setPreviousVoltage(double pv) { previousVoltage = pv; }
    void updateVoltageForNextStep() { previousVoltage = voltage; }
    bool isGround() const { return name == "0" || name == "GND" || name == "gnd"; }
};

class Element {
protected:
    Node *node1, *node2;
    string name;
public:
    Element(Node* n1, Node* n2, const string &name) : node1(n1), node2(n2), name(name) {
        if (!n1 || !n2) throw std::invalid_argument("Element nodes cannot be null for element: " + name);
    }
    virtual ~Element() = default;
    string getName() const { return name; }
    Node* getNode1() const { return node1; }
    Node* getNode2() const { return node2; }
    virtual string getType() const = 0;
    virtual string getValueAsString() const {return to_string(getValue()); }
    virtual double getValue() const { return 0.0; }
    virtual double getCurrent() const { return 0.0; }
    virtual void setCurrent(double current) { (void)current; }
    virtual void updateValue(double time) { (void)time; }
};

class Resistor : public Element {
private: double resistance;
public:
    Resistor(Node* n1, Node* n2, const string &name, double res) : Element(n1, n2, name) {
        if (res <= 0) throw std::invalid_argument("Resistance must be positive for " + name);
        this->resistance = res;
    }
    string getType() const override { return "R"; }
    double getValue() const override { return resistance; }
    string getValueAsString() const override { return to_string(resistance); }
    double getCurrent() const override {
        if (!node1 || !node2) return 0.0;
        return (node1->getVoltage() - node2->getVoltage()) / resistance;
    }
};

class BaseSource : public Element {
public:
    enum SourceType { DC, SIN, PULSE };
protected:
    double value;
    SourceType sourceType;
    double currentThroughSource;

    // Parameters for sinusoidal and pulse sources
    double dcOffset, amplitude, frequency;
    double initialValue, pulsedValue, delayTime, riseTime, fallTime, onTime, period;
public:
    BaseSource(Node* n1, Node* n2, const string& name, double val)
        : Element(n1, n2, name), value(val), sourceType(DC), currentThroughSource(0.0) {}

    void setSinParams(double offset, double amp, double freq) {
        sourceType = SIN;
        dcOffset = offset;
        amplitude = amp;
        frequency = freq;
    }

    void setPulseParams(double v1, double v2, double td, double tr, double tf, double ton, double tper) {
        sourceType = PULSE;
        initialValue = v1; pulsedValue = v2; delayTime = td;
        riseTime = tr > 0 ? tr : 1e-12; // Avoid division by zero
        fallTime = tf > 0 ? tf : 1e-12;
        onTime = ton; period = tper;
    }

    void updateValue(double time) override {
        if (sourceType == SIN) {
            value = dcOffset + amplitude * sin(2 * M_PI * frequency * time);
        } else if (sourceType == PULSE) {
            if (time < delayTime) {
                value = initialValue;
                return;
            }
            double timeInCycle = fmod(time - delayTime, period);
            if (timeInCycle <= riseTime) {
                value = initialValue + (pulsedValue - initialValue) * (timeInCycle / riseTime);
            } else if (timeInCycle <= riseTime + onTime) {
                value = pulsedValue;
            } else if (timeInCycle <= riseTime + onTime + fallTime) {
                value = pulsedValue - (pulsedValue - initialValue) * ((timeInCycle - (riseTime + onTime)) / fallTime);
            } else {
                value = initialValue;
            }
        }
    }

    double getValue() const override { return value; }
    void setValue(double val) { value = val; sourceType = DC; } // for DC sweep
    double getCurrent() const override { return currentThroughSource; }
    void setCurrent(double current) override { this->currentThroughSource = current; }
};

class VoltageSource : public BaseSource {
public:
    VoltageSource(Node* n1, Node* n2, const string &name, double dc_val)
        : BaseSource(n1, n2, name, dc_val) {}
    string getType() const override { return "V"; }
};

class CurrentSource : public BaseSource {
public:
    CurrentSource(Node* n1, Node* n2, const string &name, double dc_val)
        : BaseSource(n1, n2, name, dc_val) {}
    string getType() const override { return "I"; }
};


class Capacitor : public Element {
private: double capacitance;
public:
    Capacitor(Node* n1, Node* n2, const string &name, double cap) : Element(n1, n2, name) {
        if (cap <= 0) throw std::invalid_argument("Capacitance must be positive for " + name);
        this->capacitance = cap;
    }
    string getType() const override { return "C"; }
    double getValue() const override { return capacitance; }
    string getValueAsString() const override { return to_string(capacitance); }
};

class Inductor : public Element {
private: double inductance, current, previousCurrent;
public:
    Inductor(Node* n1, Node* n2, const string &name, double ind)
            : Element(n1, n2, name), inductance(ind), current(0.0), previousCurrent(0.0) {
        if (ind <= 0) throw std::invalid_argument("Inductance must be positive for " + name);
    }
    string getType() const override { return "L"; }
    double getValue() const override { return inductance; }
    string getValueAsString() const override { return to_string(inductance); }
    double getCurrent() const override { return current; }
    void setCurrent(double c) override { current = c; }
    double getPreviousCurrent() const { return previousCurrent; }
    void updateCurrentForNextStep() { previousCurrent = current; }
};

class IdealDiode : public Element {
public: enum State { ON, OFF };
private: double forwardVoltage; State currentState; double current; string model;
public:
    IdealDiode(Node* n1, Node* n2, const string& name, const string& model_str, double vf = 0.7)
            : Element(n1, n2, name), forwardVoltage(vf), currentState(OFF), current(0.0), model(model_str) {
        if (vf < 0) throw std::invalid_argument("Diode forward voltage must be non-negative for " + name);
    }
    string getType() const override { return "D"; }
    string getValueAsString() const override { return model; }
    double getForwardVoltage() const { return forwardVoltage; }
    State getState() const { return currentState; }
    void setState(State state) { currentState = state; }
    double getCurrent() const override { return current; }
    void setCurrent(double c) override { current = c; }
};

class VCVS : public Element {
private: Node* controlNode1; Node* controlNode2; double gain; double current;
public:
    VCVS(Node* n1, Node* n2, const string& name, Node* cn1, Node* cn2, double g)
            : Element(n1, n2, name), controlNode1(cn1), controlNode2(cn2), gain(g), current(0.0) {}
    string getType() const override { return "E"; }
    double getValue() const override { return gain; }
    string getValueAsString() const override {
        stringstream ss;
        ss << controlNode1->getName() << " " << controlNode2->getName() << " " << to_string(gain);
        return ss.str();
    }
    Node* getControlNode1() const { return controlNode1; }
    Node* getControlNode2() const { return controlNode2; }
    double getCurrent() const override { return current; }
    void setCurrent(double c) override { current = c; }
};

class VCCS : public Element {
private: Node* controlNode1; Node* controlNode2; double gain;
public:
    VCCS(Node* n1, Node* n2, const string& name, Node* cn1, Node* cn2, double g)
            : Element(n1, n2, name), controlNode1(cn1), controlNode2(cn2), gain(g) {}
    string getType() const override { return "G"; }
    double getValue() const override { return gain; }
    string getValueAsString() const override {
        stringstream ss;
        ss << controlNode1->getName() << " " << controlNode2->getName() << " " << to_string(gain);
        return ss.str();
    }
    Node* getControlNode1() const { return controlNode1; }
    Node* getControlNode2() const { return controlNode2; }
};

class CCVS : public Element {
private: string controlVoltageSourceName; double gain; double current;
public:
    CCVS(Node* n1, Node* n2, const string& name, const string& cvs_name, double g)
            : Element(n1, n2, name), controlVoltageSourceName(cvs_name), gain(g), current(0.0) {}
    string getType() const override { return "H"; }
    double getValue() const override { return gain; }
    string getValueAsString() const override {
        stringstream ss;
        ss << controlVoltageSourceName << " " << to_string(gain);
        return ss.str();
    }
    string getControlVoltageSourceName() const { return controlVoltageSourceName; }
    double getCurrent() const override { return current; }
    void setCurrent(double c) override { current = c; }
};

class CCCS : public Element {
private: string controlVoltageSourceName; double gain;
public:
    CCCS(Node* n1, Node* n2, const string& name, const string& cvs_name, double g)
            : Element(n1, n2, name), controlVoltageSourceName(cvs_name), gain(g) {}
    string getType() const override { return "F"; }
    double getValue() const override { return gain; }
    string getValueAsString() const override {
        stringstream ss;
        ss << controlVoltageSourceName << " " << to_string(gain);
        return ss.str();
    }
    string getControlVoltageSourceName() const { return controlVoltageSourceName; }
};

class MakingMNA {
private:
    vector<Node *> allNodesInCircuit;
    vector<Element *> elementsInCircuit;
    Node *groundNodeRef;
    map<Node *, int> nodeToIndexMap;
    vector<Node *> orderedNonGroundNodes;
    map<VoltageSource *, int> vsToIndexMap;
    vector<VoltageSource *> orderedVoltageSources;
    map<Inductor *, int> inductorToIndexMap;
    vector<Inductor *> orderedInductors;
    map<IdealDiode *, int> idealDiodeToIndexMap;
    vector<IdealDiode *> orderedIdealDiodes;
    map<VCVS *, int> vcvsToIndexMap;
    vector<VCVS *> orderedVCVS;
    map<CCVS *, int> ccvsToIndexMap;
    vector<CCVS *> orderedCCVS;
    double timeStep_h;

    void buildSystemMaps() {
        nodeToIndexMap.clear(); orderedNonGroundNodes.clear();
        vsToIndexMap.clear(); orderedVoltageSources.clear();
        inductorToIndexMap.clear(); orderedInductors.clear();
        idealDiodeToIndexMap.clear(); orderedIdealDiodes.clear();
        vcvsToIndexMap.clear(); orderedVCVS.clear();
        ccvsToIndexMap.clear(); orderedCCVS.clear();

        groundNodeRef = nullptr;
        for (Node *n: allNodesInCircuit) if (n->isGround()) { groundNodeRef = n; break; }
        if (!groundNodeRef && !allNodesInCircuit.empty()) throw std::runtime_error("Error: Ground node not detected in the circuit.");

        int nodeIdx = 0;
        for (Node *node: allNodesInCircuit) {
            if (!node->isGround()) {
                orderedNonGroundNodes.push_back(node);
                nodeToIndexMap[node] = nodeIdx++;
            }
        }

        for (Element *elem: elementsInCircuit) {
            if (auto vs = dynamic_cast<VoltageSource *>(elem)) orderedVoltageSources.push_back(vs);
            else if (auto ind = dynamic_cast<Inductor *>(elem)) orderedInductors.push_back(ind);
            else if (auto id = dynamic_cast<IdealDiode *>(elem)) orderedIdealDiodes.push_back(id);
            else if (auto vcvs = dynamic_cast<VCVS *>(elem)) orderedVCVS.push_back(vcvs);
            else if (auto ccvs = dynamic_cast<CCVS *>(elem)) orderedCCVS.push_back(ccvs);
        }

        for (size_t i = 0; i < orderedVoltageSources.size(); ++i) vsToIndexMap[orderedVoltageSources[i]] = i;
        for (size_t i = 0; i < orderedInductors.size(); ++i) inductorToIndexMap[orderedInductors[i]] = i;
        for (size_t i = 0; i < orderedIdealDiodes.size(); ++i) idealDiodeToIndexMap[orderedIdealDiodes[i]] = i;
        for (size_t i = 0; i < orderedVCVS.size(); ++i) vcvsToIndexMap[orderedVCVS[i]] = i;
        for (size_t i = 0; i < orderedCCVS.size(); ++i) ccvsToIndexMap[orderedCCVS[i]] = i;
    }

public:
    MakingMNA(double h = -1.0) : groundNodeRef(nullptr), timeStep_h(h) {}

    void setCircuitData(const vector<Node*>& nodes, const vector<Element*>& elements){
        this->allNodesInCircuit = nodes;
        this->elementsInCircuit = elements;
    }

    void setTimeStep(double h) { this->timeStep_h = h; }
    const vector<Node*>& getOrderedNonGroundNodes() const { return orderedNonGroundNodes; }
    const vector<Element*>& getAllElements() const { return elementsInCircuit; }
    const vector<Node*>& getAllNodesInCircuit() const { return allNodesInCircuit; }
    const vector<VoltageSource*>& getOrderedVoltageSources() const { return orderedVoltageSources; }
    const vector<Inductor*>& getOrderedInductors() const { return orderedInductors; }
    const vector<IdealDiode*>& getOrderedIdealDiodes() const { return orderedIdealDiodes; }
    const vector<VCVS*>& getOrderedVCVS() const { return orderedVCVS; }
    const vector<CCVS*>& getOrderedCCVS() const { return orderedCCVS; }

    Eigen::MatrixXd getSystemMatrixA(bool isDCAnalysis = false) {
        buildSystemMaps();

        int numNonGroundNodes = orderedNonGroundNodes.size();
        int numVS = orderedVoltageSources.size();
        int numL = orderedInductors.size();
        int numD = orderedIdealDiodes.size();
        int numVCVS = orderedVCVS.size();
        int numCCVS = orderedCCVS.size();
        int systemSize = numNonGroundNodes + numVS + numL + numD + numVCVS + numCCVS;

        if (systemSize == 0) return Eigen::MatrixXd(0,0);
        Eigen::MatrixXd A = Eigen::MatrixXd::Zero(systemSize, systemSize);

        for (Element* elem : elementsInCircuit) {
            Node* n1 = elem->getNode1();
            Node* n2 = elem->getNode2();

            if (auto res = dynamic_cast<Resistor*>(elem)) {
                double g = 1.0 / res->getValue();
                if (!n1->isGround()) A(nodeToIndexMap.at(n1), nodeToIndexMap.at(n1)) += g;
                if (!n2->isGround()) A(nodeToIndexMap.at(n2), nodeToIndexMap.at(n2)) += g;
                if (!n1->isGround() && !n2->isGround()) {
                    A(nodeToIndexMap.at(n1), nodeToIndexMap.at(n2)) -= g;
                    A(nodeToIndexMap.at(n2), nodeToIndexMap.at(n1)) -= g;
                }
            }
            else if (auto cap = dynamic_cast<Capacitor*>(elem)) {
                if (!isDCAnalysis) {
                    if (this->timeStep_h <= 0) throw std::runtime_error("Time step h is not set for capacitor " + cap->getName());
                    double g = cap->getValue() / this->timeStep_h;
                    if (!n1->isGround()) A(nodeToIndexMap.at(n1), nodeToIndexMap.at(n1)) += g;
                    if (!n2->isGround()) A(nodeToIndexMap.at(n2), nodeToIndexMap.at(n2)) += g;
                    if (!n1->isGround() && !n2->isGround()) {
                        A(nodeToIndexMap.at(n1), nodeToIndexMap.at(n2)) -= g;
                        A(nodeToIndexMap.at(n2), nodeToIndexMap.at(n1)) -= g;
                    }
                }
            }
            else if (auto vs = dynamic_cast<VoltageSource*>(elem)) {
                int i = numNonGroundNodes + vsToIndexMap.at(vs);
                if (!n1->isGround()) { A(nodeToIndexMap.at(n1), i) += 1.0; A(i, nodeToIndexMap.at(n1)) += 1.0; }
                if (!n2->isGround()) { A(nodeToIndexMap.at(n2), i) -= 1.0; A(i, nodeToIndexMap.at(n2)) -= 1.0; }
            }
            else if (auto ind = dynamic_cast<Inductor*>(elem)) {
                int i = numNonGroundNodes + numVS + inductorToIndexMap.at(ind);
                if (isDCAnalysis) {
                    if (!n1->isGround()) A(i, nodeToIndexMap.at(n1)) += 1.0;
                    if (!n2->isGround()) A(i, nodeToIndexMap.at(n2)) -= 1.0;
                } else {
                    double l_div_h = ind->getValue() / this->timeStep_h;
                    if (!n1->isGround()) { A(nodeToIndexMap.at(n1), i) += 1.0; A(i, nodeToIndexMap.at(n1)) += 1.0; }
                    if (!n2->isGround()) { A(nodeToIndexMap.at(n2), i) -= 1.0; A(i, nodeToIndexMap.at(n2)) -= 1.0; }
                    A(i, i) -= l_div_h;
                }
            }
            else if (auto diode = dynamic_cast<IdealDiode*>(elem)) {
                int i = numNonGroundNodes + numVS + numL + idealDiodeToIndexMap.at(diode);
                if (diode->getState() == IdealDiode::ON) {
                    if (!n1->isGround()) { A(nodeToIndexMap.at(n1), i) += 1.0; A(i, nodeToIndexMap.at(n1)) += 1.0; }
                    if (!n2->isGround()) { A(nodeToIndexMap.at(n2), i) -= 1.0; A(i, nodeToIndexMap.at(n2)) -= 1.0; }
                } else {
                    A(i, i) = 1.0;
                }
            }
            else if (auto vccs = dynamic_cast<VCCS*>(elem)) {
                double g = vccs->getValue();
                Node* cn1 = vccs->getControlNode1(); Node* cn2 = vccs->getControlNode2();
                if (!n1->isGround() && !cn1->isGround()) A(nodeToIndexMap.at(n1), nodeToIndexMap.at(cn1)) += g;
                if (!n1->isGround() && !cn2->isGround()) A(nodeToIndexMap.at(n1), nodeToIndexMap.at(cn2)) -= g;
                if (!n2->isGround() && !cn1->isGround()) A(nodeToIndexMap.at(n2), nodeToIndexMap.at(cn1)) -= g;
                if (!n2->isGround() && !cn2->isGround()) A(nodeToIndexMap.at(n2), nodeToIndexMap.at(cn2)) += g;
            }
            else if (auto cccs = dynamic_cast<CCCS*>(elem)) {
                double gain = cccs->getValue();
                VoltageSource* ctrl_vs = nullptr;
                for(auto* vs_ptr : orderedVoltageSources) if(vs_ptr->getName() == cccs->getControlVoltageSourceName()) ctrl_vs = vs_ptr;
                if (!ctrl_vs) throw std::runtime_error("Error: Dependent source '" + cccs->getName() + "' has an undefined control element '" + cccs->getControlVoltageSourceName() + "'.");
                int ctrl_i = numNonGroundNodes + vsToIndexMap.at(ctrl_vs);
                if (!n1->isGround()) A(nodeToIndexMap.at(n1), ctrl_i) += gain;
                if (!n2->isGround()) A(nodeToIndexMap.at(n2), ctrl_i) -= gain;
            }
            else if (auto vcvs = dynamic_cast<VCVS*>(elem)) {
                int i = numNonGroundNodes + numVS + numL + numD + vcvsToIndexMap.at(vcvs);
                Node* cn1 = vcvs->getControlNode1(); Node* cn2 = vcvs->getControlNode2();
                double gain = vcvs->getValue();
                if (!n1->isGround()) { A(nodeToIndexMap.at(n1), i) += 1.0; A(i, nodeToIndexMap.at(n1)) += 1.0; }
                if (!n2->isGround()) { A(nodeToIndexMap.at(n2), i) -= 1.0; A(i, nodeToIndexMap.at(n2)) -= 1.0; }
                if (!cn1->isGround()) A(i, nodeToIndexMap.at(cn1)) -= gain;
                if (!cn2->isGround()) A(i, nodeToIndexMap.at(cn2)) += gain;
            }
            else if (auto ccvs = dynamic_cast<CCVS*>(elem)) {
                int i = numNonGroundNodes + numVS + numL + numD + numVCVS + ccvsToIndexMap.at(ccvs);
                double gain = ccvs->getValue();
                VoltageSource* ctrl_vs = nullptr;
                for(auto* vs_ptr : orderedVoltageSources) if(vs_ptr->getName() == ccvs->getControlVoltageSourceName()) ctrl_vs = vs_ptr;
                if (!ctrl_vs) throw std::runtime_error("Error: Dependent source '" + ccvs->getName() + "' has an undefined control element '" + ccvs->getControlVoltageSourceName() + "'.");
                int ctrl_i = numNonGroundNodes + vsToIndexMap.at(ctrl_vs);
                if (!n1->isGround()) {A(i, nodeToIndexMap.at(n1)) += 1.0;} else {A(i, nodeToIndexMap.at(n1))=0;};
                if (!n2->isGround()) {A(i, nodeToIndexMap.at(n2)) -= 1.0;} else {A(i, nodeToIndexMap.at(n2))=0;};
                A(i, ctrl_i) -= gain;
            }
        }
        return A;
    }

    Eigen::VectorXd getSystemVectorZ(bool isDCAnalysis = false, double time = 0.0) {
        int systemSize = orderedNonGroundNodes.size() + orderedVoltageSources.size() + orderedInductors.size() + orderedIdealDiodes.size() + orderedVCVS.size() + orderedCCVS.size();
        if (systemSize == 0) return Eigen::VectorXd(0);
        Eigen::VectorXd Z = Eigen::VectorXd::Zero(systemSize);

        for (Element* elem : elementsInCircuit) {
             if (auto cs = dynamic_cast<CurrentSource*>(elem)) {
                cs->updateValue(time);
                if (!cs->getNode1()->isGround()) Z(nodeToIndexMap.at(cs->getNode1())) -= cs->getValue();
                if (!cs->getNode2()->isGround()) Z(nodeToIndexMap.at(cs->getNode2())) += cs->getValue();
            } else if (auto cap = dynamic_cast<Capacitor*>(elem)) {
                if (!isDCAnalysis) {
                    if (this->timeStep_h <= 0) throw std::runtime_error("Time step h is not set for capacitor " + cap->getName());
                    double c_div_h = cap->getValue() / this->timeStep_h;
                    double v_n1_prev = cap->getNode1()->getPreviousVoltage();
                    double v_n2_prev = cap->getNode2()->getPreviousVoltage();
                    double i_eq_cap = c_div_h * (v_n1_prev - v_n2_prev);
                    if (!cap->getNode1()->isGround()) Z(nodeToIndexMap.at(cap->getNode1())) += i_eq_cap;
                    if (!cap->getNode2()->isGround()) Z(nodeToIndexMap.at(cap->getNode2())) -= i_eq_cap;
                }
            }
        }

        for (const auto& vs : orderedVoltageSources) {
            vs->updateValue(time);
            Z(orderedNonGroundNodes.size() + vsToIndexMap.at(vs)) = vs->getValue();
        }
        for (const auto& ind : orderedInductors) {
            int i = orderedNonGroundNodes.size() + orderedVoltageSources.size() + inductorToIndexMap.at(ind);
            if (!isDCAnalysis) {
                Z(i) = - (ind->getValue() / this->timeStep_h) * ind->getPreviousCurrent();
            }
        }
        for (const auto& diode : orderedIdealDiodes) {
            int i = orderedNonGroundNodes.size() + orderedVoltageSources.size() + orderedInductors.size() + idealDiodeToIndexMap.at(diode);
            if (diode->getState() == IdealDiode::ON) Z(i) = diode->getForwardVoltage();
        }
        return Z;
    }
};

class MNASolver {
public:
    MNASolver() {}
    Eigen::VectorXd solve(const Eigen::MatrixXd& A, const Eigen::VectorXd& Z) {
        if (A.rows() == 0 && Z.size() == 0) return Eigen::VectorXd(0);
        if (A.rows() != A.cols() || A.rows() != Z.size()) throw std::runtime_error("Matrix/vector dimensions incompatible.");
        if (A.rows() == 0) throw std::runtime_error("System of equations is empty.");

        Eigen::PartialPivLU<Eigen::MatrixXd> lu(A);
        if (A.rows() > 0 && std::abs(lu.determinant()) < 1e-12) {
             throw std::runtime_error("Error: System matrix is singular or ill-conditioned. The circuit may not be solvable (e.g. floating nodes or voltage source loop).");
        }
        return lu.solve(Z);
    }

    void updateCircuitState(const Eigen::VectorXd& X, MakingMNA& mnaCircuit) {
        const auto& nonGroundNodes = mnaCircuit.getOrderedNonGroundNodes();
        const auto& voltageSources = mnaCircuit.getOrderedVoltageSources();
        const auto& inductors = mnaCircuit.getOrderedInductors();
        const auto& idealDiodes = mnaCircuit.getOrderedIdealDiodes();
        const auto& vcvs_sources = mnaCircuit.getOrderedVCVS();
        const auto& ccvs_sources = mnaCircuit.getOrderedCCVS();

        int numNonGroundNodes = nonGroundNodes.size();
        int numVS = voltageSources.size();
        int numL = inductors.size();
        int numD = idealDiodes.size();
        int numVCVS = vcvs_sources.size();

        if (static_cast<size_t>(X.size()) != numNonGroundNodes+numVS+numL+numD+numVCVS+ccvs_sources.size()) {
            throw std::runtime_error("Solution vector size does not match number of unknowns.");
        }

        for (size_t i = 0; i < nonGroundNodes.size(); ++i) nonGroundNodes[i]->setVoltage(X(i));
        for (size_t i = 0; i < voltageSources.size(); ++i) voltageSources[i]->setCurrent(X(numNonGroundNodes + i));
        for (size_t i = 0; i < inductors.size(); ++i) inductors[i]->setCurrent(X(numNonGroundNodes + numVS + i));
        for (size_t i = 0; i < idealDiodes.size(); ++i) idealDiodes[i]->setCurrent(X(numNonGroundNodes + numVS + numL + i));
        for (size_t i = 0; i < vcvs_sources.size(); ++i) vcvs_sources[i]->setCurrent(X(numNonGroundNodes + numVS + numL + numD + i));
        for (size_t i = 0; i < ccvs_sources.size(); ++i) ccvs_sources[i]->setCurrent(X(numNonGroundNodes + numVS + numL + numD + numVCVS + i));
    }
};

// Forward declaration
class centralController;

class TransientAnalysis {
private:
    MakingMNA& mnaCircuit;
    MNASolver solver;
    centralController& controller;
    double t_step, t_stop;
public:
    TransientAnalysis(MakingMNA& circuit, centralController& ctrl, double step, double stop)
            : mnaCircuit(circuit), solver(), controller(ctrl), t_step(step), t_stop(stop) {
        if (t_step <= 0 || t_stop <= 0 || t_step > t_stop) {
            throw std::invalid_argument("Invalid time parameters for transient analysis.");
        }
    }

    void run(const vector<string>& output_vars);
};

class DCSweepAnalysis {
private:
    MakingMNA& mnaCircuit;
    MNASolver solver;
    centralController& controller;
    string sweepComponentName;
    double startValue, endValue, increment;
public:
    DCSweepAnalysis(MakingMNA& circuit, centralController& ctrl, const string& compName, double start, double end, double inc)
            : mnaCircuit(circuit), solver(), controller(ctrl), sweepComponentName(compName), startValue(start), endValue(end), increment(inc) {
        if (increment == 0) throw std::invalid_argument("DC sweep increment cannot be zero.");
        if ((endValue > startValue && increment < 0) || (endValue < startValue && increment > 0)) {
            increment = -increment;
        }
    }
    void run(const vector<string>& output_vars);
};

// ============================================================================================
// ||                  بخش کنترلر و نمایش (Controller & View)                                 ||
// ============================================================================================

class CommandParser;
string trim(const string& str);
double stringToDouble(const string& s_in);


struct AnalysisParams {
    string type;
    vector<string> params;
};

class centralController {
private:
    MakingMNA circuit_manager;
    map<string, Node*> nodes_map;
    vector<Element*> elements_list;
    map<string, Element*> elements_map;
    AnalysisParams last_analysis;
    bool is_in_file_menu = false;
    vector<string> schematic_files;
    string schematics_directory = "./circuits";
    string schematics_index_file = schematics_directory + "/schematics_index.txt";

    Node* getNode(const string& name) {
        if (nodes_map.find(name) == nodes_map.end()) {
            nodes_map[name] = new Node(name);
        }
        return nodes_map[name];
    }

    void cleanupUnusedNodes() {
        if (elements_list.empty()) {
            vector<string> nodes_to_delete;
            for (auto const& [name, node_ptr] : nodes_map) {
                if (!node_ptr->isGround()) {
                    nodes_to_delete.push_back(name);
                }
            }
            for (const auto& name : nodes_to_delete) {
                delete nodes_map[name];
                nodes_map.erase(name);
            }
            return;
        }

        set<Node*> used_nodes;
        for (const auto& elem : elements_list) {
            used_nodes.insert(elem->getNode1());
            used_nodes.insert(elem->getNode2());
        }

        vector<string> nodes_to_delete;
        for (auto const& [name, node_ptr] : nodes_map) {
            if (used_nodes.find(node_ptr) == used_nodes.end() && !node_ptr->isGround()) {
                nodes_to_delete.push_back(name);
            }
        }

        for (const auto& name : nodes_to_delete) {
            cout << "Info: Deleting unused node " << name << "." << endl;
            delete nodes_map[name];
            nodes_map.erase(name);
        }
    }

    void loadAvailableSchematics() {
        schematic_files.clear();
        ifstream index_file(schematics_index_file);
        if (index_file.is_open()) {
            string line;
            while (getline(index_file, line)) {
                line = trim(line);
                if (!line.empty()) {
                    schematic_files.push_back(line);
                }
            }
            index_file.close();
        } else {
            ofstream new_index(schematics_index_file);
            if (!new_index.is_open()) {
                 cerr << "Warning: Could not access schematics directory '" << schematics_directory << "'. Please ensure it exists." << endl;
            } else {
                 new_index.close();
            }
        }
        sort(schematic_files.begin(), schematic_files.end());
    }


public:
    centralController() { loadAvailableSchematics(); }
    ~centralController() { clearCircuit(); }

    bool nodeExists(const string& name) const { return nodes_map.count(name) > 0; }
    Node* getNodePtr(const string& name) const {
        if (nodes_map.count(name)) return nodes_map.at(name);
        return nullptr;
    }
    bool elementExists(const string& name) const { return elements_map.count(name) > 0; }
    Element* getElementPtr(const string& name) const {
        if (elements_map.count(name)) return elements_map.at(name);
        return nullptr;
    }


    void clearCircuit() {
        for (auto const& [name, node_ptr] : nodes_map) delete node_ptr;
        for (auto elem_ptr : elements_list) delete elem_ptr;
        nodes_map.clear();
        elements_list.clear();
        elements_map.clear();
        cout << "Current circuit cleared." << endl;
    }

    bool isInFileMenu() const { return is_in_file_menu; }
    void setFileMenuState(bool state) { is_in_file_menu = state; }

    void handleShowExistingSchematicsCommand() {
        is_in_file_menu = true;
        loadAvailableSchematics();

        cout << "-choose existing schematic:" << endl;
        if (schematic_files.empty()) {
            cout << " (No schematics found. Use 'NewFile <name>' to save one.)" << endl;
        } else {
            for (size_t i = 0; i < schematic_files.size(); ++i) {
                cout << i + 1 << "-" << schematic_files[i] << endl;
            }
        }
        cout << " (Enter number to load, 'NewFile <name>' to save current, or 'return' to exit)" << endl;
    }

    void handleChooseSchematic(const string& choice, CommandParser* parser);

    void handleNewFileCommand(const string& filename) {
        if (filename.empty() || filename.find_first_of("\\/:*?\"<>|") != string::npos) {
            cout << "Error: Invalid filename." << endl;
            return;
        }

        string full_path = schematics_directory + "/" + filename + ".txt";
        ofstream file(full_path);
        if (!file.is_open()) {
            cout << "Error: Could not create file " << full_path << ". Please ensure the 'circuits' directory exists." << endl;
            return;
        }

        for (const auto* elem : elements_list) {
             file << elem->getName() << " " << elem->getNode1()->getName() << " " << elem->getNode2()->getName() << " " << elem->getValueAsString() << endl;
        }
        file.close();

        loadAvailableSchematics();
        bool found = false;
        for (const auto& fname : schematic_files) if (fname == filename) { found = true; break; }

        if (!found) {
            ofstream index_file(schematics_index_file, ios_base::app);
            if (index_file.is_open()) { index_file << filename << endl; index_file.close();
            } else { cout << "Warning: Could not update schematics index file." << endl; }
        }

        cout << "SUCCESS: Current circuit saved to '" << filename << ".txt'." << endl;
        if (is_in_file_menu) handleShowExistingSchematicsCommand();
    }


    void makingResistor(const string& name, const string& n1_name, const string& n2_name, const string& val_str) {
        if (elements_map.count(name)) throw logic_error("Error: Element " + name + " already exists in the circuit");
        elements_map[name] = new Resistor(getNode(n1_name), getNode(n2_name), name, stringToDouble(val_str));
        elements_list.push_back(elements_map[name]);
    }

    void makingCapacity(const string& name, const string& n1_name, const string& n2_name, const string& val_str) {
        if (elements_map.count(name)) throw logic_error("Error: Element " + name + " already exists in the circuit");
        elements_map[name] = new Capacitor(getNode(n1_name), getNode(n2_name), name, stringToDouble(val_str));
        elements_list.push_back(elements_map[name]);
    }

    void makingInductor(const string& name, const string& n1_name, const string& n2_name, const string& val_str) {
        if (elements_map.count(name)) throw logic_error("Error: Element " + name + " already exists in the circuit");
        elements_map[name] = new Inductor(getNode(n1_name), getNode(n2_name), name, stringToDouble(val_str));
        elements_list.push_back(elements_map[name]);
    }

    void makingDiode(const string& name, const string& n1_name, const string& n2_name, const string& model) {
        if (elements_map.count(name)) throw logic_error("Error: Element " + name + " already exists in the circuit");
        elements_map[name] = new IdealDiode(getNode(n1_name), getNode(n2_name), name, model);
        elements_list.push_back(elements_map[name]);
    }

    void makingVoltageSourceDC(const string& name, const string& n_plus, const string& n_minus, const string& val_str) {
        if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
        elements_map[name] = new VoltageSource(getNode(n_plus), getNode(n_minus), name, stringToDouble(val_str));
        elements_list.push_back(elements_map[name]);
    }

    void makingCurrentSourceDC(const string& name, const string& n_plus, const string& n_minus, const string& val_str) {
        if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
        elements_map[name] = new CurrentSource(getNode(n_plus), getNode(n_minus), name, stringToDouble(val_str));
        elements_list.push_back(elements_map[name]);
    }

    void makingVoltageSourceSin(const string& name, const string& n_plus, const string& n_minus, const string& v_off, const string& v_amp, const string& freq) {
         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
        auto* vs = new VoltageSource(getNode(n_plus), getNode(n_minus), name, 0.0);
        vs->setSinParams(stringToDouble(v_off), stringToDouble(v_amp), stringToDouble(freq));
        elements_map[name] = vs;
        elements_list.push_back(elements_map[name]);
    }

    void makingCurrentSourceSin(const string& name, const string& n_plus, const string& n_minus, const string& i_off, const string& i_amp, const string& freq) {
        if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
        auto* cs = new CurrentSource(getNode(n_plus), getNode(n_minus), name, 0.0);
        cs->setSinParams(stringToDouble(i_off), stringToDouble(i_amp), stringToDouble(freq));
        elements_map[name] = cs;
        elements_list.push_back(elements_map[name]);
    }

    void makingVoltageSourcePulse(const string& name, const string& n_plus, const string& n_minus,
                                  const string& v1, const string& v2, const string& td,
                                  const string& tr, const string& tf, const string& pw,
                                  const string& period) {
        if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
        auto* vs = new VoltageSource(getNode(n_plus), getNode(n_minus), name, 0.0);
        vs->setPulseParams(stringToDouble(v1), stringToDouble(v2), stringToDouble(td), stringToDouble(tr), stringToDouble(tf), stringToDouble(pw), stringToDouble(period));
        elements_map[name] = vs;
        elements_list.push_back(elements_map[name]);
    }

    void makingCurrentSourcePulse(const string& name, const string& n_plus, const string& n_minus,
                                  const string& i1, const string& i2, const string& td,
                                  const string& tr, const string& tf, const string& pw,
                                  const string& period) {
        if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
        auto* cs = new CurrentSource(getNode(n_plus), getNode(n_minus), name, 0.0);
        cs->setPulseParams(stringToDouble(i1), stringToDouble(i2), stringToDouble(td), stringToDouble(tr), stringToDouble(tf), stringToDouble(pw), stringToDouble(period));
        elements_map[name] = cs;
        elements_list.push_back(elements_map[name]);
    }

    void makingVCVS(const string& name, const string& n_p, const string& n_m, const string& nc_p, const string& nc_m, const string& gain_str) {
        if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
        elements_map[name] = new VCVS(getNode(n_p), getNode(n_m), name, getNode(nc_p), getNode(nc_m), stringToDouble(gain_str));
        elements_list.push_back(elements_map[name]);
    }

    void makingVCCS(const string& name, const string& n_p, const string& n_m, const string& nc_p, const string& nc_m, const string& gain_str) {
         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
        elements_map[name] = new VCCS(getNode(n_p), getNode(n_m), name, getNode(nc_p), getNode(nc_m), stringToDouble(gain_str));
        elements_list.push_back(elements_map[name]);
    }

    void makingCCVS(const string& name, const string& n_p, const string& n_m, const string& vctrl_name, const string& gain_str) {
        if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
        if (!elements_map.count(vctrl_name) || dynamic_cast<VoltageSource*>(elements_map[vctrl_name]) == nullptr) {
            throw logic_error("Error: Control voltage source " + vctrl_name + " not found for CCVS " + name);
        }
        elements_map[name] = new CCVS(getNode(n_p), getNode(n_m), name, vctrl_name, stringToDouble(gain_str));
        elements_list.push_back(elements_map[name]);
    }

    void makingCCCS(const string& name, const string& n_p, const string& n_m, const string& vctrl_name, const string& gain_str) {
        if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
        if (!elements_map.count(vctrl_name) || dynamic_cast<VoltageSource*>(elements_map[vctrl_name]) == nullptr) {
            throw logic_error("Error: Control voltage source " + vctrl_name + " not found for CCCS " + name);
        }
        elements_map[name] = new CCCS(getNode(n_p), getNode(n_m), name, vctrl_name, stringToDouble(gain_str));
        elements_list.push_back(elements_map[name]);
    }

    void deletingElement(const string& name_to_delete) {
        if (!elements_map.count(name_to_delete)) throw logic_error("Error: Cannot delete component; component '" + name_to_delete + "' not found");

        for (const auto& elem : elements_list) {
            if (auto ccvs = dynamic_cast<CCVS*>(elem)) {
                if (ccvs->getControlVoltageSourceName() == name_to_delete) {
                    throw logic_error("Error: Cannot delete component '" + name_to_delete + "' because it is a control branch for source '" + ccvs->getName() + "'.");
                }
            } else if (auto cccs = dynamic_cast<CCCS*>(elem)) {
                 if (cccs->getControlVoltageSourceName() == name_to_delete) {
                    throw logic_error("Error: Cannot delete component '" + name_to_delete + "' because it is a control branch for source '" + cccs->getName() + "'.");
                }
            }
        }

        Element* elem_to_delete_ptr = elements_map[name_to_delete];
        elements_map.erase(name_to_delete);
        elements_list.erase(remove(elements_list.begin(), elements_list.end(), elem_to_delete_ptr), elements_list.end());

        delete elem_to_delete_ptr;
        cout << "SUCCESS: Component " << name_to_delete << " deleted." << endl;
        cleanupUnusedNodes();
    }

    void addingGround(const string& node_name){
        getNode(node_name);
        cout << "SUCCESS: Node " << node_name << " is designated as a ground reference." << endl;
    }

    void handleNodesCommand() {
        if (nodes_map.empty()) { cout << "No nodes in the circuit yet." << endl; return; }
        cout << "Available nodes:" << endl;
        for(auto const& [name, node_ptr] : nodes_map) { cout << name << " "; }
        cout << endl;
    }

    void handleListCommand(const string& componentType = "") {
        if (elements_list.empty()) { cout << "No elements in the circuit yet." << endl; return; }
        bool found = false;
        cout << "Circuit elements:" << endl;
        for(auto const& elem : elements_list) {
            bool type_match = false;
            string elem_type_prefix = elem->getName().substr(0,1);
            if (componentType.empty()) type_match = true;
            else if (componentType == elem_type_prefix) type_match = true;

            if (type_match) {
                cout << "- " << elem->getName() << " connected between " << elem->getNode1()->getName() << " and " << elem->getNode2()->getName() << endl;
                found = true;
            }
        }
        if (!found) cout << "No elements of type '" << componentType << "' found." << endl;
    }

    void handleRenameNodeCommand(const string& oldName, const string& newName) {
        if (nodes_map.find(oldName) == nodes_map.end()) {
            throw logic_error("ERROR: Node " + oldName + " does not exist in the circuit");
        }
        if (nodes_map.find(newName) != nodes_map.end()) {
            throw logic_error("ERROR: Node name " + newName + " already exists");
        }
        Node* node_ptr = nodes_map[oldName];
        node_ptr->setName(newName);
        nodes_map.erase(oldName);
        nodes_map[newName] = node_ptr;
        cout << "SUCCESS: Node renamed from " << oldName << " to " << newName << endl;
    }

    void defineAnalysis(const string& type, const vector<string>& params) {
        last_analysis.type = type;
        last_analysis.params = params;
        cout << "SUCCESS: " << type << " analysis defined." << endl;
    }

    void handlePrintCommand(const string& analysisType,
                            const vector<string>& analysisParams,
                            const string& outputVarsStr) {

        vector<string> output_vars;
        stringstream ss(outputVarsStr);
        string temp;
        while (ss >> temp) { output_vars.push_back(temp); }

        if (output_vars.empty()){ throw logic_error("Error: Missing output variables for print command."); }

        const AnalysisParams* analysis_to_run = nullptr;
        AnalysisParams temp_analysis;

        if (!analysisParams.empty()){
            temp_analysis.type = analysisType; temp_analysis.params = analysisParams;
            analysis_to_run = &temp_analysis;
        } else if (last_analysis.type == analysisType) {
            analysis_to_run = &last_analysis;
        } else {
            throw logic_error("Error: No analysis parameters provided or defined for " + analysisType);
        }

        vector<Node*> nodes_vector;
        for(auto const& [name, ptr] : nodes_map) nodes_vector.push_back(ptr);
        circuit_manager.setCircuitData(nodes_vector, elements_list);


        if (analysis_to_run->type == "TRAN") {
            if (analysis_to_run->params.size() < 2) throw logic_error("Error: Not enough parameters for TRAN analysis.");
            double tstep = stringToDouble(analysis_to_run->params[0]);
            double tstop = stringToDouble(analysis_to_run->params[1]);
            TransientAnalysis tran(circuit_manager, *this, tstep, tstop);
            tran.run(output_vars);

        } else if (analysis_to_run->type == "DC") {
            if (analysis_to_run->params.size() < 4) throw logic_error("Error: Not enough parameters for DC analysis.");
            string srcName = analysis_to_run->params[0];
            double start = stringToDouble(analysis_to_run->params[1]);
            double end = stringToDouble(analysis_to_run->params[2]);
            double inc = stringToDouble(analysis_to_run->params[3]);
            DCSweepAnalysis dc(circuit_manager, *this, srcName, start, end, inc);
            dc.run(output_vars);
        }
    }
};

// ============================================================================================
// ||                  بخش پردازشگر دستورات (View)                                             ||
// ============================================================================================

string trim(const string& str) {
    const string whitespace = " \t\n\r\f\v";
    size_t start = str.find_first_not_of(whitespace);
    if (string::npos == start) return "";
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

// BUG FIX: Replaced valuate, checkDouble, checkingNemadElmi with a single robust function
double stringToDouble(const string& s_in) {
    string s = trim(s_in);
    if (s.empty()) {
        throw std::invalid_argument("Cannot convert empty string to double.");
    }

    size_t suffix_start = string::npos;
    for (size_t i = 0; i < s.length(); ++i) {
        char c = s[i];
        if (isdigit(c) || c == '.' || (i == 0 && (c == '-' || c == '+'))) {
            continue;
        }
        if ((c == 'e' || c == 'E') && i < s.length() - 1) {
            char next_c = s[i+1];
            if (isdigit(next_c) || ((next_c == '-' || next_c == '+') && i < s.length() - 2)) {
                i++;
                continue;
            }
        }
        suffix_start = i;
        break;
    }

    string num_part = (suffix_start == string::npos) ? s : s.substr(0, suffix_start);
    string suffix_part = (suffix_start == string::npos) ? "" : s.substr(suffix_start);
    std::transform(suffix_part.begin(), suffix_part.end(), suffix_part.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    double value;
    try {
        value = stod(num_part);
    } catch (const std::exception& e) {
        throw std::invalid_argument("Invalid numeric part in value: '" + s_in + "'");
    }

    if (suffix_part.empty() || suffix_part == "v" || suffix_part == "a" || suffix_part == "s" || suffix_part == "h" || suffix_part == "f" || suffix_part == "hz") {
        return value;
    }
    if (suffix_part == "g" || suffix_part == "ghz") return value * 1e9;
    if (suffix_part == "meg") return value * 1e6;
    if (suffix_part == "k" || suffix_part == "khz") return value * 1e3;
    if (suffix_part == "m" || suffix_part == "ms") return value * 1e-3;
    if (suffix_part == "u" || suffix_part == "us") return value * 1e-6;
    if (suffix_part == "n" || suffix_part == "ns") return value * 1e-9;
    if (suffix_part == "p" || suffix_part == "ps") return value * 1e-12;
    if (suffix_part == "f" || suffix_part == "fs") return value * 1e-15;

    throw std::invalid_argument("Unrecognized SI suffix '" + suffix_part + "' in value '" + s_in + "'");
}


class CommandParser {
private:
    centralController& controller;

    void validateParameter(const string& param_name, const string& param_val_raw, const string& context_name, bool allow_zero, bool must_be_positive, bool must_be_non_negative) {
        double val = stringToDouble(param_val_raw); // stringToDouble will throw on invalid format
        if (must_be_positive && val <= 0) {
            throw logic_error("Error: Parameter " + param_name + " must be positive in " + context_name + " (" + param_val_raw + ")");
        }
        if (must_be_non_negative && val < 0) {
             throw logic_error("Error: Parameter " + param_name + " cannot be negative in " + context_name + " (" + param_val_raw + ")");
        }
        if (!allow_zero && val == 0) {
             throw logic_error("Error: Parameter " + param_name + " cannot be zero in " + context_name + " (" + param_val_raw + ")");
        }
    }

    bool tryParseNetlistLine(const string& in) {
        smatch matches;
        regex netlist_pattern(R"(^([a-zA-Z]\S*)\s+(\S+)\s+(\S+)\s+(.+)$)");
        if (regex_match(in, matches, netlist_pattern)) {
            string command = "add " + in;
            try {
                if (tryParseElementCommands(command) || tryParseSourceCommands(command)) { return true; }
            } catch (const exception& e) {
                 cerr << "Error processing line from file: '" << in << "'. Reason: " << e.what() << endl;
            }
        }
        return false;
    }


    bool tryParseElementCommands(const string& in) {
        smatch matches;
        regex add_res_pattern(R"(^add\s+(R\S*)\s+(\S+)\s+(\S+)\s+(.+)$)");
        if (regex_match(in, matches, add_res_pattern)) {
            validateParameter("Resistance", matches[4].str(), "Resistor " + matches[1].str(), false, true, false);
            controller.makingResistor(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
        }

        regex add_cap_pattern(R"(^add\s+(C\S*)\s+(\S+)\s+(\S+)\s+(.+)$)");
        if (regex_match(in, matches, add_cap_pattern)) {
            validateParameter("Capacitance", matches[4].str(), "Capacitor " + matches[1].str(), false, true, false);
            controller.makingCapacity(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
        }

        regex add_ind_pattern(R"(^add\s+(L\S*)\s+(\S+)\s+(\S+)\s+(.+)$)");
        if (regex_match(in, matches, add_ind_pattern)) {
            validateParameter("Inductance", matches[4].str(), "Inductor " + matches[1].str(), false, true, false);
            controller.makingInductor(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
        }

        regex add_diode_pattern(R"(^add\s+(D\S*)\s+(\S+)\s+(\S+)\s+(D|Z)$)");
        if (regex_match(in, matches, add_diode_pattern)) {
            controller.makingDiode(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
        }

        regex add_gnd_pattern(R"(^add\s+GND\s+(\S+)$)");
        if (regex_match(in, matches, add_gnd_pattern)) {
            controller.addingGround(matches[1].str()); return true;
        }

        regex del_elem_pattern(R"(^delete\s+([RCLD]\S*)$)");
        if(regex_match(in, matches, del_elem_pattern)) {
            controller.deletingElement(matches[1].str()); return true;
        }

        return false;
    }

    bool tryParseSourceCommands(const string& in) {
        smatch matches;
        regex add_src_generic_pattern(R"(^add\s+([VI]\S*)\s+(\S+)\s+(\S+)\s+(.+)$)");
        if (regex_match(in, matches, add_src_generic_pattern)) {
            string name = matches[1].str();
            char type = name[0];
            string n1 = matches[2].str();
            string n2 = matches[3].str();
            string val_full_str = trim(matches[4].str());

            // BUG FIX: Updated regex for SIN to handle space separators
            regex sin_params_pattern(R"(^SIN\s*\(\s*(\S+)\s+(\S+)\s+(\S+)\s*\)$)");
            smatch sin_matches;
            if (regex_match(val_full_str, sin_matches, sin_params_pattern)) {
                validateParameter("Frequency", sin_matches[3].str(), "SIN source " + name, false, true, false);
                if (type == 'V') controller.makingVoltageSourceSin(name, n1, n2, sin_matches[1].str(), sin_matches[2].str(), sin_matches[3].str());
                else controller.makingCurrentSourceSin(name, n1, n2, sin_matches[1].str(), sin_matches[2].str(), sin_matches[3].str());
                return true;
            }

            regex pulse_params_pattern(R"(^PULSE\s*\(\s*(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s*\)$)");
            smatch pulse_matches;
            if (regex_match(val_full_str, pulse_matches, pulse_params_pattern)) {
                validateParameter("Td", pulse_matches[3].str(), "PULSE source " + name, true, false, true);
                validateParameter("Tr", pulse_matches[4].str(), "PULSE source " + name, false, true, false);
                validateParameter("Tf", pulse_matches[5].str(), "PULSE source " + name, false, true, false);
                validateParameter("PW", pulse_matches[6].str(), "PULSE source " + name, true, false, true);
                validateParameter("Period", pulse_matches[7].str(), "PULSE source " + name, false, true, false);
                if(stringToDouble(pulse_matches[4].str()) + stringToDouble(pulse_matches[5].str()) > stringToDouble(pulse_matches[7].str())){
                    throw logic_error("Error: Sum of rise and fall times must be less than period for PULSE source " + name);
                }

                if (type == 'V') controller.makingVoltageSourcePulse(name, n1, n2, pulse_matches[1].str(), pulse_matches[2].str(), pulse_matches[3].str(), pulse_matches[4].str(), pulse_matches[5].str(), pulse_matches[6].str(), pulse_matches[7].str());
                else controller.makingCurrentSourcePulse(name, n1, n2, pulse_matches[1].str(), pulse_matches[2].str(), pulse_matches[3].str(), pulse_matches[4].str(), pulse_matches[5].str(), pulse_matches[6].str(), pulse_matches[7].str());
                return true;
            }

            validateParameter("Value", val_full_str, "DC source " + name, true, false, false);
            if(type == 'V') controller.makingVoltageSourceDC(name, n1, n2, val_full_str);
            else controller.makingCurrentSourceDC(name, n1, n2, val_full_str);
            return true;
        }

        // Dependent Sources
        regex add_vcvs_pattern(R"(^add\s+(E\S*)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(.+)$)");
        if (regex_match(in, matches, add_vcvs_pattern)) {
             controller.makingVCVS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str(), matches[6].str()); return true;
        }
        regex add_vccs_pattern(R"(^add\s+(G\S*)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(.+)$)");
        if (regex_match(in, matches, add_vccs_pattern)) {
            controller.makingVCCS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str(), matches[6].str()); return true;
        }
        regex add_ccvs_pattern(R"(^add\s+(H\S*)\s+(\S+)\s+(\S+)\s+(V\S+)\s+(.+)$)");
        if (regex_match(in, matches, add_ccvs_pattern)) {
            controller.makingCCVS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str()); return true;
        }
        regex add_cccs_pattern(R"(^add\s+(F\S*)\s+(\S+)\s+(\S+)\s+(V\S+)\s+(.+)$)");
        if (regex_match(in, matches, add_cccs_pattern)) {
            controller.makingCCCS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str()); return true;
        }
        regex del_src_pattern(R"(^delete\s+([VIEGHF]\S*)$)");
        if(regex_match(in, matches, del_src_pattern)) {
            controller.deletingElement(matches[1].str()); return true;
        }
        return false;
    }

    bool tryParseAnalysisDefinitionCommands(const string& in) {
        smatch matches;
        regex tran_def_pattern(R"(^\.TRAN\s+(\S+)\s+(\S+)(?:\s+(\S+))?(?:\s+(\S+))?\s*$)");
        if (regex_match(in, matches, tran_def_pattern)) {
            vector<string> params = {matches[1].str(), matches[2].str()};
            if(matches[3].matched) params.push_back(matches[3].str());
            if(matches[4].matched) params.push_back(matches[4].str());
            controller.defineAnalysis("TRAN", params); return true;
        }

        regex dc_def_pattern(R"(^\.DC\s+([VI]\w*)\s+(\S+)\s+(\S+)\s+(\S+)\s*$)");
        if (regex_match(in, matches, dc_def_pattern)) {
            vector<string> params = {matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()};
            controller.defineAnalysis("DC", params); return true;
        }
        return false;
    }

    bool tryParseNodeCommands(const string& in) {
        if (in == "nodes") { controller.handleNodesCommand(); return true; }

        smatch matches;
        regex rename_node_full_pattern(R"(^rename\s+node\s+(\S+)\s+(\S+)$)");
        if (regex_match(in, matches, rename_node_full_pattern)) {
            controller.handleRenameNodeCommand(matches[1].str(), matches[2].str()); return true;
        } else if (in.rfind("rename node", 0) == 0) {
            throw logic_error("ERROR: Invalid syntax correct format: rename node <old_name> <new_name>");
        }
        return false;
    }

    bool tryParseListCommands(const string& in) {
        if (in == "list") { controller.handleListCommand(); return true; }

        smatch matches;
        regex list_comp_pattern(R"(^list\s+([RCLDVIEGHF])$)");
        if (regex_match(in, matches, list_comp_pattern)) {
            controller.handleListCommand(matches[1].str()); return true;
        }
        return false;
    }

    bool tryParsePrintCommands(const string& in) {
        smatch matches;
        regex print_base_pattern(R"(^print\s+(TRAN|DC|AC)\s*(.*)$)");
        if (regex_match(in, matches, print_base_pattern)) {
            string analysisType = matches[1].str();
            string remaining_args = trim(matches[2].str());
            vector<string> analysisParams;
            string outputVarsStr;

            stringstream ss(remaining_args);
            string word;
            vector<string> words;
            while(ss >> word) words.push_back(word);

            size_t var_start_index = 0;
            if (analysisType == "TRAN" && words.size() >= 2) {
                try {
                    stringToDouble(words[0]); stringToDouble(words[1]);
                    analysisParams.push_back(words[0]);
                    analysisParams.push_back(words[1]);
                    var_start_index = 2;
                } catch(...) {}
            } else if (analysisType == "DC" && words.size() >= 4) {
                 try {
                    stringToDouble(words[1]); stringToDouble(words[2]); stringToDouble(words[3]);
                    analysisParams = {words[0], words[1], words[2], words[3]};
                    var_start_index = 4;
                 } catch(...) {}
            }

            stringstream vars_ss;
            for(size_t i = var_start_index; i < words.size(); ++i) { vars_ss << words[i] << " "; }
            outputVarsStr = vars_ss.str();

            controller.handlePrintCommand(analysisType, analysisParams, outputVarsStr);
            return true;
        }
        return false;
    }

    bool tryParseFileAndMenuCommands(const string& in) {
        if (in == "show existing schematics") { controller.handleShowExistingSchematicsCommand(); return true; }
        smatch matches;
        regex new_file_pattern(R"(^NewFile\s+(\S+)$)");
        if (regex_match(in, matches, new_file_pattern)) { controller.handleNewFileCommand(matches[1].str()); return true; }
        if(controller.isInFileMenu()){ controller.handleChooseSchematic(in, this); return true; }
        return false;
    }


public:
    explicit CommandParser(centralController& ctrl) : controller(ctrl) {}

    void processInput(const string& in) {
        string trimmed_in = trim(in);
        if (trimmed_in.empty()) return;

        if (tryParseFileAndMenuCommands(trimmed_in)) return;
        if (tryParseAnalysisDefinitionCommands(trimmed_in)) return;
        if (tryParseNodeCommands(trimmed_in)) return;
        if (tryParseListCommands(trimmed_in)) return;
        if (tryParsePrintCommands(trimmed_in)) return;
        if (tryParseElementCommands(trimmed_in)) return;
        if (tryParseSourceCommands(trimmed_in)) return;
        if (tryParseNetlistLine(trimmed_in)) return; // For processing file lines

        throw logic_error("Error: Unknown command or invalid syntax. -> " + in);
    }
};

void centralController::handleChooseSchematic(const string& choice, CommandParser* parser) {
    if (choice == "return") {
        is_in_file_menu = false;
        cout << "Returning to main menu..." << endl;
        return;
    }

    try {
        size_t choice_num = stoul(choice);
        if (choice_num > 0 && choice_num <= schematic_files.size()) {
            string filename = schematic_files[choice_num - 1];
            string full_path = schematics_directory + "/" + filename + ".txt";

            ifstream file(full_path);
            if (!file.is_open()) { throw logic_error("Error: Could not open file " + filename); }

            clearCircuit();
            cout << "Loading schematic '" << filename << "'..." << endl;
            cout << filename << ".txt:" << endl;

            setFileMenuState(false);

            string line;
            while(getline(file, line)) {
                line = trim(line);
                if (line.empty() || line[0] == '*' || line[0] == '.') continue;
                cout << line << endl;
                try {
                    parser->processInput(line);
                } catch (const exception& e) {
                    cerr << "Error processing line from file: '" << line << "' -> " << e.what() << endl;
                }
            }
            file.close();
            setFileMenuState(true);

        } else {
            cout << "-Error: Inappropriate input" << endl;
        }
    } catch (const exception& e) {
        cout << "-Error: Inappropriate input" << endl;
    }

    handleShowExistingSchematicsCommand();
}

void TransientAnalysis::run(const vector<string>& output_vars) {
    for (const auto& var : output_vars) {
        if (var[0] == 'V' && var[1] == '(') {
            string node_name = var.substr(2, var.length() - 3);
            if (!controller.nodeExists(node_name)) {
                throw logic_error("Error: Node '" + node_name + "' not found in circuit for TRAN analysis.");
            }
        } else if (var[0] == 'I' && var[1] == '(') {
            string elem_name = var.substr(2, var.length() - 3);
            if (!controller.elementExists(elem_name)) {
                throw logic_error("Error: Component '" + elem_name + "' not found in circuit for TRAN analysis.");
            }
        }
    }

    double current_time = 0.0;
    cout << "Time(s)\t";
    for (const auto& var : output_vars) cout << var << "\t";
    cout << endl << string(output_vars.size() * 10 + 10, '-') << endl;

    mnaCircuit.setTimeStep(t_step);

    while (current_time <= t_stop + 1e-9) {
        Eigen::MatrixXd A = mnaCircuit.getSystemMatrixA(false);
        Eigen::VectorXd Z = mnaCircuit.getSystemVectorZ(false, current_time);
        Eigen::VectorXd X = solver.solve(A, Z);
        solver.updateCircuitState(X, mnaCircuit);

        cout << fixed << setprecision(6) << current_time << "\t";
        for (const auto& var : output_vars) {
             if (var[0] == 'V' && var[1] == '(') {
                string node_name = var.substr(2, var.length() - 3);
                cout << controller.getNodePtr(node_name)->getVoltage() << "\t";
            } else if (var[0] == 'I' && var[1] == '(') {
                string elem_name = var.substr(2, var.length() - 3);
                cout << controller.getElementPtr(elem_name)->getCurrent() << "\t";
            }
        }
        cout << endl;

        current_time += t_step;

        for (auto* node : mnaCircuit.getAllNodesInCircuit()) node->updateVoltageForNextStep();
        for (auto* ind : mnaCircuit.getOrderedInductors()) ind->updateCurrentForNextStep();
    }
}

void DCSweepAnalysis::run(const vector<string>& output_vars) {
    for (const auto& var : output_vars) {
        if (var[0] == 'V' && var[1] == '(') {
            string node_name = var.substr(2, var.length() - 3);
            if (!controller.nodeExists(node_name)) {
                throw logic_error("Error: Node '" + node_name + "' not found in circuit for DC analysis.");
            }
        } else if (var[0] == 'I' && var[1] == '(') {
            string elem_name = var.substr(2, var.length() - 3);
            if (!controller.elementExists(elem_name)) {
                throw logic_error("Error: Component '" + elem_name + "' not found in circuit for DC analysis.");
            }
        }
    }

    Element* sweepElement = controller.getElementPtr(sweepComponentName);
    if (!sweepElement) throw std::runtime_error("Sweep component '" + sweepComponentName + "' not found.");
    auto* sweepVoltageSource = dynamic_cast<VoltageSource*>(sweepElement);
    if (!sweepVoltageSource) throw std::runtime_error("Sweep component must be a VoltageSource.");

    cout << sweepComponentName << "\t";
    for (const auto& var : output_vars) cout << var << "\t";
    cout << endl << string(output_vars.size() * 10 + 10, '-') << endl;

    for (double val = startValue; (increment > 0 ? val <= endValue + 1e-9 : val >= endValue - 1e-9) ; val += increment) {
        sweepVoltageSource->setValue(val);

        Eigen::MatrixXd A = mnaCircuit.getSystemMatrixA(true);
        Eigen::VectorXd Z = mnaCircuit.getSystemVectorZ(true);
        Eigen::VectorXd X = solver.solve(A, Z);
        solver.updateCircuitState(X, mnaCircuit);

        cout << fixed << setprecision(6) << val << "\t";
        for (const auto& var : output_vars) {
             if (var[0] == 'V' && var[1] == '(') {
                string node_name = var.substr(2, var.length() - 3);
                cout << controller.getNodePtr(node_name)->getVoltage() << "\t";
            } else if (var[0] == 'I' && var[1] == '(') {
                string elem_name = var.substr(2, var.length() - 3);
                cout << controller.getElementPtr(elem_name)->getCurrent() << "\t";
            }
        }
        cout << endl;
    }
}

// ============================================================================================
// ||                  تابع اصلی (Main Function)                                              ||
// ============================================================================================
int main() {
    centralController controller_instance;
    CommandParser parser(controller_instance);

    cout << "Welcome to Circuit Simulator!" << endl;
    cout << "Use 'show existing schematics' to manage files." << endl;
    cout << "Please ensure the 'circuits' directory exists in the same folder as the executable." << endl;
    cout << "Enter commands or 'exit' to quit." << endl;

    string line;
    while (cout << ">>> " && getline(cin, line) && line != "exit") {
        try {
            parser.processInput(line);
        } catch (const exception& e) {
            cerr << e.what() << endl;
        }
    }

    cout << "Exiting simulator. Goodbye!" << endl;
    return 0;
}
