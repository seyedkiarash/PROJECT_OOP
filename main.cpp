// // ///////////////////////////////////////////////
// // // #include <iomanip>   // برای форматирование вывода (std::fixed, std::setprecision)
// // // #include <iostream>
// // // #include <map>       // برای نگاشت گره‌ها به اندیس ماتریس
// // // #include <stdexcept> // برای کلاس‌های استثنا مانند std::invalid_argument
// // // #include <string>
// // // #include <vector>
// // //
// // // // کتابخانه Eigen برای عملیات ماتریسی
// // // #include "Eigen/Dense"
// // //
// // // // استفاده از فضای نام std برای سادگی در این مثال
// // // // در پروژه‌های بزرگتر، بهتر است از پیشوند std:: استفاده شود یا using declarations محدودتری به کار رود.
// // // using namespace std;
// // //
// // // // تعریف کلاس گره (Node)
// // // class Node {
// // // private:
// // //     double voltage; // ولتاژ گره، به double تغییر داده شد
// // //     string name;    // نام گره
// // //
// // // public:
// // //     // سازنده کلاس گره
// // //     Node(const string &name, double voltage = 0.0) { // مقدار پیش‌فرض ولتاژ 0.0 در نظر گرفته شد
// // //         this->name = name;
// // //         this->voltage = voltage;
// // //     }
// // //
// // //     // دریافت نام گره
// // //     string getName() const {
// // //         return name;
// // //     }
// // //
// // //     // دریافت ولتاژ گره
// // //     double getVoltage() const {
// // //         return voltage;
// // //     }
// // //
// // //     // تنظیم ولتاژ گره
// // //     void setVoltage(double v) {
// // //         voltage = v;
// // //     }
// // //
// // //     // بررسی اینکه آیا گره، گره زمین (گراند) است یا خیر
// // //     bool isGround() const {
// // //         return name == "0" || name == "GND" || name == "gnd"; // نام‌های رایج برای زمین
// // //     }
// // // };
// // //
// // // // تعریف کلاس پایه برای المان‌های مداری (Element)
// // // class Element {
// // // protected:
// // //     Node *node1, *node2; // اشاره‌گر به گره‌های متصل به المان
// // //     string name;         // نام المان
// // //
// // // public:
// // //     // سازنده کلاس المان
// // //     Element(Node* n1, Node* n2, const string &name) {
// // //         if (!n1 || !n2) {
// // //             throw std::invalid_argument("گره‌های المان نمی‌توانند تهی (null) باشند.");
// // //         }
// // //         this->node1 = n1;
// // //         this->node2 = n2;
// // //         this->name = name;
// // //     }
// // //
// // //     // مخرب مجازی پیش‌فرض
// // //     virtual ~Element() = default;
// // //
// // //     // دریافت نام المان
// // //     string getName() const { return name; }
// // //
// // //     // دریافت گره اول
// // //     Node* getNode1() const { return node1; }
// // //
// // //     // دریافت گره دوم
// // //     Node* getNode2() const { return node2; }
// // //
// // //     // تابع مجازی خالص برای دریافت نوع المان (باید در کلاس‌های مشتق پیاده‌سازی شود)
// // //     virtual string getType() const = 0;
// // //
// // //     // تابع مجازی خالص برای دریافت مقدار مشخصه المان (مقاومت، ولتاژ منبع، جریان منبع و ...)
// // //     virtual double getValue() const = 0;
// // //
// // //     // تابع مجازی برای دریافت جریان عبوری از المان (می‌تواند در کلاس‌های مشتق بازنویسی شود)
// // //     // برای المان‌هایی مانند مقاومت پس از حل مدار محاسبه می‌شود
// // //     // برای منابع ولتاژ، این یکی از مجهولات سیستم MNA است
// // //     virtual double getCurrent() const {
// // //         return 0.0; // پیاده‌سازی پیش‌فرض
// // //     }
// // //
// // //     // تابع مجازی برای تنظیم جریان عبوری از المان (مربوط به منابع ولتاژ)
// // //     virtual void setCurrent(double current) {
// // //         // پیاده‌سازی پیش‌فرض، برای جلوگیری از هشدار پارامتر استفاده‌نشده
// // //         (void)current;
// // //     }
// // // };
// // //
// // // // کلاس مقاومت (Resistor) مشتق‌شده از Element
// // // class Resistor : public Element {
// // // private:
// // //     double resistance; // مقدار مقاومت
// // //
// // // public:
// // //     Resistor(Node* n1, Node* n2, const string &name, double res) : Element(n1, n2, name) {
// // //         if (res <= 0) {
// // //             // مطابق فایل PDF، بخش ۱۹ (مدیریت خطاها)
// // //             throw std::invalid_argument("خطا: مقدار مقاومت باید مثبت باشد. المان: " + name);
// // //         }
// // //         this->resistance = res;
// // //     }
// // //
// // //     string getType() const override { return "Resistor"; }
// // //     double getValue() const override { return resistance; } // مقدار مقاومت را برمی‌گرداند
// // //
// // //     // جریان عبوری از مقاومت طبق قانون اهم (پس از مشخص شدن ولتاژ گره‌ها)
// // //     double getCurrent() const override {
// // //         if (node1 && node2) {
// // //             return (node1->getVoltage() - node2->getVoltage()) / resistance;
// // //         }
// // //         return 0.0;
// // //     }
// // // };
// // //
// // // // کلاس منبع ولتاژ مستقل (VoltageSource) مشتق‌شده از Element
// // // class VoltageSource : public Element {
// // // private:
// // //     double voltageValue;         // مقدار ولتاژ منبع
// // //     double currentThroughSource; // جریان عبوری از منبع (یکی از مجهولات MNA)
// // //
// // // public:
// // //     VoltageSource(Node* n1, Node* n2, const string &name, double val) : Element(n1, n2, name) {
// // //         this->voltageValue = val;
// // //         this->currentThroughSource = 0.0; // مقدار اولیه
// // //     }
// // //
// // //     string getType() const override { return "VoltageSource"; }
// // //     double getValue() const override { return voltageValue; } // مقدار ولتاژ تنظیم‌شده منبع را برمی‌گرداند
// // //
// // //     double getCurrent() const override {
// // //         return currentThroughSource;
// // //     }
// // //     void setCurrent(double current) override {
// // //         this->currentThroughSource = current;
// // //     }
// // // };
// // //
// // // // کلاس منبع جریان مستقل (CurrentSource) مشتق‌شده از Element
// // // class CurrentSource : public Element {
// // // private:
// // //     double currentValue; // مقدار جریان منبع
// // //
// // // public:
// // //     // گره n1 گرهی است که جریان از آن خارج می‌شود، گره n2 گرهی است که جریان به آن وارد می‌شود
// // //     CurrentSource(Node* n1, Node* n2, const string &name, double val) : Element(n1, n2, name) {
// // //         this->currentValue = val;
// // //     }
// // //
// // //     string getType() const override { return "CurrentSource"; }
// // //     double getValue() const override { return currentValue; } // مقدار جریان تنظیم‌شده منبع را برمی‌گرداند
// // //
// // //     // ولتاژ دو سر منبع جریان توسط مدار تعیین می‌شود و جزو مجهولات مستقیم برای این المان نیست
// // // };
// // //
// // //
// // // // کلاس مسئول تشکیل ماتریس‌های MNA
// // // class MakingMNA {
// // // private:
// // //     vector<Node*> allNodesInCircuit;     // لیست تمام گره‌های موجود در مدار
// // //     vector<Element*> elementsInCircuit;  // لیست تمام المان‌های موجود در مدار
// // //     Node* groundNodeRef;                 // اشاره‌گر به گره زمین
// // //
// // //     // نگاشت‌ها برای تشکیل ماتریس
// // //     map<Node*, int> nodeToIndexMap;          // نگاشت گره‌های غیرزمین به اندیس سطر/ستون در ماتریس MNA
// // //     vector<Node*> orderedNonGroundNodes;     // لیست مرتب‌شده گره‌های غیرزمین
// // //     map<VoltageSource*, int> vsToIndexMap;   // نگاشت منابع ولتاژ به اندیس مجهول جریانشان در MNA
// // //     vector<VoltageSource*> orderedVoltageSources; // لیست مرتب‌شده منابع ولتاژ
// // //
// // //     // متد داخلی برای ساخت نگاشت‌ها
// // //     void buildNodeAndVoltageSourceMaps() {
// // //         nodeToIndexMap.clear();
// // //         orderedNonGroundNodes.clear();
// // //         vsToIndexMap.clear();
// // //         orderedVoltageSources.clear();
// // //
// // //         // تعیین گره زمین در صورت عدم تنظیم صریح
// // //         if (!groundNodeRef) {
// // //             for (Node* n : allNodesInCircuit) {
// // //                 if (n->isGround()) {
// // //                     groundNodeRef = n;
// // //                     break;
// // //                 }
// // //             }
// // //             if (!groundNodeRef) {
// // //                 // مطابق فایل PDF، بخش ۱۹ (مدیریت خطاها)
// // //                 throw std::runtime_error("خطا: گره زمین (گراند) در مدار تشخیص داده نشد. تحلیل امکان‌پذیر نیست.");
// // //             }
// // //         }
// // //
// // //         // شماره‌گذاری گره‌های غیرزمین
// // //         int nodeIdx = 0;
// // //         for (Node* node : allNodesInCircuit) {
// // //             if (node != groundNodeRef) {
// // //                 orderedNonGroundNodes.push_back(node);
// // //                 nodeToIndexMap[node] = nodeIdx++;
// // //             }
// // //         }
// // //
// // //         // شماره‌گذاری منابع ولتاژ (برای مجهولات جریان آنها)
// // //         int vsIdx = 0;
// // //         for (Element* elem : elementsInCircuit) {
// // //             if (auto vs = dynamic_cast<VoltageSource*>(elem)) {
// // //                 orderedVoltageSources.push_back(vs);
// // //                 vsToIndexMap[vs] = vsIdx++;
// // //             }
// // //         }
// // //     }
// // //
// // // public:
// // //     // سازنده
// // //     MakingMNA() : groundNodeRef(nullptr) {}
// // //
// // //     // مخرب (در صورت نیاز به مدیریت حافظه پیچیده‌تر، باید تکمیل شود)
// // //     ~MakingMNA() {
// // //         // اگر گره‌ها و المان‌ها در این کلاس new شده باشند، باید delete شوند.
// // //         // در این پیاده‌سازی، فرض بر این است که مدیریت حافظه آنها خارج از این کلاس انجام می‌شود.
// // //     }
// // //
// // //     // افزودن گره به مدار
// // //     void addNode(Node* node) {
// // //         if (!node) return;
// // //         allNodesInCircuit.push_back(node);
// // //         if (node->isGround()) {
// // //             if (groundNodeRef != nullptr && groundNodeRef != node) {
// // //                 cout << "هشدار: چندین گره زمین تعریف شده است. از اولین گره زمین شناسایی‌شده استفاده می‌شود: "
// // //                      << groundNodeRef->getName() << endl;
// // //             } else if (groundNodeRef == nullptr) {
// // //                 groundNodeRef = node;
// // //             }
// // //         }
// // //     }
// // //
// // //     // افزودن المان به مدار
// // //     void addElement(Element* element) {
// // //         if (!element) return;
// // //         elementsInCircuit.push_back(element);
// // //     }
// // //
// // //     // تنظیم صریح گره زمین
// // //     void setGroundNode(Node* gnd) {
// // //         if (!gnd) {
// // //             throw std::invalid_argument("گره زمین نمی‌تواند تهی (null) باشد.");
// // //         }
// // //         bool found = false;
// // //         for(Node* n : allNodesInCircuit) {
// // //             if(n == gnd) {
// // //                 found = true;
// // //                 break;
// // //             }
// // //         }
// // //         if(!found) addNode(gnd); // اگر گره زمین هنوز به لیست گره‌ها اضافه نشده، اضافه کن
// // //
// // //         groundNodeRef = gnd;
// // //     }
// // //
// // //     // تشکیل و دریافت ماتریس سیستم A در معادله Ax = Z
// // //     Eigen::MatrixXd getSystemMatrixA() {
// // //         buildNodeAndVoltageSourceMaps(); // اطمینان از به‌روز بودن نگاشت‌ها
// // //
// // //         int numNonGroundNodes = orderedNonGroundNodes.size();
// // //         int numVoltageSources = orderedVoltageSources.size();
// // //         int systemSize = numNonGroundNodes + numVoltageSources;
// // //
// // //         if (systemSize == 0) {
// // //             // اگر مداری خالی یا فقط با یک گره زمین باشد
// // //              throw std::runtime_error("خطا: مدار برای تحلیل بسیار کوچک است (فاقد گره‌های غیر زمین یا منابع ولتاژ).");
// // //         }
// // //
// // //         Eigen::MatrixXd A = Eigen::MatrixXd::Zero(systemSize, systemSize);
// // //
// // //         // بخش G (مربوط به هدایت‌ها - مقاومت‌ها)
// // //         for (Element* elem : elementsInCircuit) {
// // //             if (auto res = dynamic_cast<Resistor*>(elem)) {
// // //                 double conductance = 1.0 / res->getValue();
// // //                 Node* n1 = res->getNode1();
// // //                 Node* n2 = res->getNode2();
// // //
// // //                 // تاثیر بر قطر اصلی
// // //                 if (n1 != groundNodeRef) {
// // //                     A(nodeToIndexMap[n1], nodeToIndexMap[n1]) += conductance;
// // //                 }
// // //                 if (n2 != groundNodeRef) {
// // //                     A(nodeToIndexMap[n2], nodeToIndexMap[n2]) += conductance;
// // //                 }
// // //                 // تاثیر بر عناصر خارج از قطر
// // //                 if (n1 != groundNodeRef && n2 != groundNodeRef) {
// // //                     A(nodeToIndexMap[n1], nodeToIndexMap[n2]) -= conductance;
// // //                     A(nodeToIndexMap[n2], nodeToIndexMap[n1]) -= conductance;
// // //                 }
// // //             }
// // //         }
// // //
// // //         // بخش B و C (مربوط به منابع ولتاژ)
// // //         // طبق توضیحات فایل PDF بخش ۵.۲
// // //         for (size_t i = 0; i < orderedVoltageSources.size(); ++i) {
// // //             VoltageSource* vs = orderedVoltageSources[i];
// // //             Node* n_plus = vs->getNode1();  // گره مثبت منبع ولتاژ
// // //             Node* n_minus = vs->getNode2(); // گره منفی منبع ولتاژ
// // //             int vsMNAIndex = vsToIndexMap[vs]; // اندیس مجهول جریان این منبع ولتاژ
// // //
// // //             if (n_plus != groundNodeRef) {
// // //                 int nodeIdx = nodeToIndexMap[n_plus];
// // //                 A(nodeIdx, numNonGroundNodes + vsMNAIndex) += 1.0;  // بخش B
// // //                 A(numNonGroundNodes + vsMNAIndex, nodeIdx) += 1.0;  // بخش C
// // //             }
// // //             if (n_minus != groundNodeRef) {
// // //                 int nodeIdx = nodeToIndexMap[n_minus];
// // //                 A(nodeIdx, numNonGroundNodes + vsMNAIndex) -= 1.0; // بخش B
// // //                 A(numNonGroundNodes + vsMNAIndex, nodeIdx) -= 1.0; // بخش C
// // //             }
// // //         }
// // //
// // //         // بخش D ماتریس A برای منابع ولتاژ مستقل ایده‌آل صفر است.
// // //         // Eigen::MatrixXd::Zero قبلاً این بخش را صفر کرده است.
// // //
// // //         return A;
// // //     }
// // //
// // //     // تشکیل و دریافت بردار سمت راست Z (یا B در برخی منابع) در معادله Ax = Z
// // //     Eigen::VectorXd getSystemVectorZ() {
// // //         // فرض می‌شود buildNodeAndVoltageSourceMaps() قبلاً فراخوانی شده است
// // //         int numNonGroundNodes = orderedNonGroundNodes.size();
// // //         int numVoltageSources = orderedVoltageSources.size();
// // //         int systemSize = numNonGroundNodes + numVoltageSources;
// // //
// // //         if (systemSize == 0 && numNonGroundNodes == 0) { // بررسی مجدد برای حالت خاص بردار Z
// // //              // اگرچه getSystemMatrixA این را بررسی می‌کند، برای اطمینان
// // //              Eigen::VectorXd Z_empty(0);
// // //              return Z_empty;
// // //         }
// // //
// // //
// // //         Eigen::VectorXd Z = Eigen::VectorXd::Zero(systemSize);
// // //
// // //         // بخش J (مربوط به منابع جریان)
// // //         // طبق توضیحات فایل PDF بخش ۵.۲
// // //         for (Element* elem : elementsInCircuit) {
// // //             if (auto cs = dynamic_cast<CurrentSource*>(elem)) {
// // //                 Node* n_from = cs->getNode1(); // جریان از این گره خارج می‌شود
// // //                 Node* n_to = cs->getNode2();   // جریان به این گره وارد می‌شود
// // //                 double currentValue = cs->getValue();
// // //
// // //                 if (n_to != groundNodeRef) { // جریان ورودی به گره n_to
// // //                     Z(nodeToIndexMap[n_to]) += currentValue;
// // //                 }
// // //                 if (n_from != groundNodeRef) { // جریان خروجی از گره n_from
// // //                     Z(nodeToIndexMap[n_from]) -= currentValue;
// // //                 }
// // //             }
// // //         }
// // //
// // //         // بخش E (مربوط به مقادیر منابع ولتاژ)
// // //         // طبق توضیحات فایل PDF بخش ۵.۲
// // //         for (size_t i = 0; i < orderedVoltageSources.size(); ++i) {
// // //             VoltageSource* vs = orderedVoltageSources[i];
// // //             int vsMNAIndex = vsToIndexMap[vs];
// // //             Z(numNonGroundNodes + vsMNAIndex) = vs->getValue();
// // //         }
// // //         return Z;
// // //     }
// // //
// // //     // متدهای Getter برای دسترسی به لیست‌های داخلی (برای به‌روزرسانی وضعیت پس از حل)
// // //     const vector<Node*>& getOrderedNonGroundNodes() const {
// // //         return orderedNonGroundNodes;
// // //     }
// // //
// // //     const vector<VoltageSource*>& getOrderedVoltageSources() const {
// // //         return orderedVoltageSources;
// // //     }
// // //     // برای دسترسی به نگاشت vs->index برای بروزرسانی جریان vs ها
// // //      const map<VoltageSource*, int>& getVoltageSourceToIndexMap() const {
// // //         return vsToIndexMap;
// // //     }
// // //      const vector<Element*>& getAllElements() const { // برای محاسبه جریان مقاومت‌ها پس از حل
// // //         return elementsInCircuit;
// // //     }
// // // };
// // //
// // // // کلاس حل‌کننده سیستم معادلات MNA
// // // class MNASolver {
// // // public:
// // //     MNASolver() {}
// // //
// // //     // حل سیستم AX = Z و برگرداندن بردار X (شامل ولتاژ گره‌ها و سپس جریان منابع ولتاژ)
// // //     Eigen::VectorXd solve(const Eigen::MatrixXd& A, const Eigen::VectorXd& Z) {
// // //         if (A.rows() != A.cols() || A.rows() != Z.size()) {
// // //             throw std::runtime_error("خطا: ابعاد ماتریس و بردار برای حل سازگار نیستند.");
// // //         }
// // //         if (A.rows() == 0) {
// // //              throw std::runtime_error("خطا: سیستم معادلات برای حل خالی است.");
// // //         }
// // //
// // //         // استفاده از تجزیه LU با Partial Pivoting برای پایداری عددی
// // //         Eigen::PartialPivLU<Eigen::MatrixXd> lu(A);
// // //         if (!lu.isInvertible()) {
// // //             // این خطا می‌تواند برای مدارهای تعریف‌نشده (مثلاً بخش‌های شناور) یا ماتریس‌های تکین رخ دهد.
// // //             // مطابق فایل PDF، بخش ۱۹ (مدیریت خطاها - مدار ناپیوسته)
// // //             throw std::runtime_error("خطا: ماتریس سیستم تکین یا بدحالت است. مدار ممکن است قابل حل نباشد (مثلاً بخش‌های شناور، منابع ولتاژ اضافی و ...).");
// // //         }
// // //         return lu.solve(Z);
// // //     }
// // //
// // //     // به‌روزرسانی ولتاژ گره‌ها و جریان منابع ولتاژ پس از حل سیستم
// // //     void updateCircuitState(const Eigen::VectorXd& X, MakingMNA& mnaCircuit) {
// // //         const auto& nonGroundNodes = mnaCircuit.getOrderedNonGroundNodes();
// // //         const auto& voltageSources = mnaCircuit.getOrderedVoltageSources(); // این لیست مرتب است
// // //         const auto& vsMap = mnaCircuit.getVoltageSourceToIndexMap(); // برای گرفتن اندیس صحیح هر vs
// // //
// // //         int numNonGroundNodes = nonGroundNodes.size();
// // //
// // //         // به‌روزرسانی ولتاژ گره‌ها
// // //         for (int i = 0; i < numNonGroundNodes; ++i) {
// // //             nonGroundNodes[i]->setVoltage(X(i));
// // //         }
// // //
// // //         // به‌روزرسانی جریان عبوری از منابع ولتاژ
// // //         // ترتیب در orderedVoltageSources با ترتیب اندیس‌ها در vsMap مطابقت دارد
// // //         for (size_t i = 0; i < voltageSources.size(); ++i) {
// // //             VoltageSource* vs = voltageSources[i]; // vs فعلی از لیست مرتب شده
// // //             int vsMNAIndex = vsMap.at(vs); // اندیس این vs در بخش جریان‌های مجهول ماتریس MNA
// // //             vs->setCurrent(X(numNonGroundNodes + vsMNAIndex));
// // //         }
// // //     }
// // // };
// // //
// // //
// // // // تابع اصلی برای تست
// // // int main() {
// // //     // تنظیم دقت خروجی برای اعداد اعشاری
// // //     cout << fixed << setprecision(6);
// // //
// // //     // ۱. ایجاد گره‌ها
// // //     Node n1("1"), n2("2"), n_gnd("0"); // گره "0" یا "GND" معمولاً زمین است
// // //
// // //     // ۲. ایجاد مدیر مدار و افزودن گره‌ها
// // //     MakingMNA circuit;
// // //     circuit.addNode(&n1);
// // //     circuit.addNode(&n2);
// // //     circuit.addNode(&n_gnd);
// // //     // circuit.setGroundNode(&n_gnd); // تنظیم صریح گره زمین (اگرچه به طور خودکار نیز شناسایی می‌شود)
// // //
// // //     // ۳. ایجاد المان‌ها و افزودن آنها به مدار
// // //     try {
// // //         // مثال از فایل PDF بخش ۹ (مدار RC): V1=5V, R=1k, C=1uF (اینجا C را با مقاومت دوم جایگزین می‌کنیم برای مثال DC)
// // //         // V1 بین گره 1 و زمین، R1 بین 1 و 2، R2 بین 2 و زمین
// // //         VoltageSource vs(&n1, &n_gnd, "V1", 5.0);   // منبع ولتاژ 5 ولت بین گره 1 و زمین
// // //         Resistor r1(&n1, &n2, "R1", 1000.0);     // مقاومت 1 کیلواهم بین گره 1 و 2
// // //         Resistor r2(&n2, &n_gnd, "R2", 2000.0);   // مقاومت 2 کیلواهم بین گره 2 و زمین
// // //         // CurrentSource cs(&n_gnd, &n2, "I1", 0.001); // مثال: منبع جریان 1 میلی‌آمپر از زمین به گره 2
// // //
// // //         circuit.addElement(&vs);
// // //         circuit.addElement(&r1);
// // //         circuit.addElement(&r2);
// // //         // circuit.addElement(&cs);
// // //
// // //         // ۴. دریافت ماتریس‌های MNA
// // //         Eigen::MatrixXd A = circuit.getSystemMatrixA();
// // //         Eigen::VectorXd Z = circuit.getSystemVectorZ();
// // //
// // //         cout << "ماتریس سیستم A:\n" << A << endl << endl;
// // //         cout << "بردار سیستم Z:\n" << Z << endl << endl;
// // //
// // //         // ۵. حل سیستم
// // //         MNASolver solver;
// // //         Eigen::VectorXd X = solver.solve(A, Z);
// // //
// // //         cout << "بردار حل X (شامل ولتاژ گره‌ها و سپس جریان منابع ولتاژ):\n" << X << endl << endl;
// // //
// // //         // ۶. به‌روزرسانی ولتاژ گره‌ها و جریان منابع ولتاژ در اشیاء مربوطه
// // //         solver.updateCircuitState(X, circuit);
// // //
// // //         // ۷. چاپ نتایج
// // //         cout << "ولتاژ گره‌ها پس از حل:" << endl;
// // //         for (const auto* node : circuit.getOrderedNonGroundNodes()) {
// // //             cout << "گره " << node->getName() << ": " << node->getVoltage() << " V" << endl;
// // //         }
// // //
// // //         cout << "\nجریان عبوری از منابع ولتاژ:" << endl;
// // //         for (const auto* vs_elem : circuit.getOrderedVoltageSources()) {
// // //             cout << "جریان عبوری از " << vs_elem->getName() << ": " << vs_elem->getCurrent() << " A" << endl;
// // //         }
// // //
// // //         cout << "\nجریان عبوری از مقاومت‌ها (محاسبه‌شده پس از حل):" << endl;
// // //         for (const auto* elem : circuit.getAllElements()) {
// // //            if (const Resistor* res = dynamic_cast<const Resistor*>(elem)) {
// // //                cout << "جریان عبوری از " << res->getName() << " (" << res->getNode1()->getName() << "->" << res->getNode2()->getName() << "): "
// // //                     << res->getCurrent() << " A" << endl;
// // //            }
// // //         }
// // //
// // //
// // //     } catch (const std::exception& e) {
// // //         cerr << "یک خطا رخ داد: " << e.what() << endl;
// // //         // مثال‌هایی از پیام‌های خطا در فایل PDF:
// // //         // "خطا: مقدار مقاومت نمی‌تواند صفر یا منفی باشد."
// // //         // "خطا: گره زمین (گراند) در مدار تشخیص داده نشد."
// // //         // "خطا: مقدار منفی یا صفر برای یک المان نامعتبر است."
// // //         // "خطا: ماتریس سیستم تکین یا بدحالت است..."
// // //     }
// // //
// // //     return 0;
// // // }
// //
// // ////////////////////////////////////////////////
// // #include <iostream>
// // #include <vector>
// // #include <string>
// // #include <stdexcept> // برای کلاس‌های استثنا مانند std::invalid_argument
// // #include <map>       // برای نگاشت گره‌ها به اندیس ماتریس
// // #include <iomanip>   // برای форматирование вывода (std::fixed, std::setprecision)
// // #include <cmath>     // برای std::abs و std::fabs
// // #include <algorithm> // برای std::swap
// // #include <regex>
// //
// // // استفاده از فضای نام std برای سادگی در این مثال
// // // در پروژه‌های بزرگتر، بهتر است از پیشوند std:: استفاده شود یا using declarations محدودتری به کار رود.
// // using namespace std;
// //
// // // تعریف نوع برای ماتریس و بردار
// // using Matrix = vector<vector<double>>;
// // using Vector = vector<double>;
// //
// // // تعریف کلاس گره (Node)
// // class Node {
// // private:
// //     double voltage; // ولتاژ گره، به double تغییر داده شد
// //     string name;    // نام گره
// //
// // public:
// //     // سازنده کلاس گره
// //     Node(const string &name, double voltage = 0.0) { // مقدار پیش‌فرض ولتاژ 0.0 در نظر گرفته شد
// //         this->name = name;
// //         this->voltage = voltage;
// //     }
// //
// //     // دریافت نام گره
// //     string getName() const {
// //         return name;
// //     }
// //
// //     // دریافت ولتاژ گره
// //     double getVoltage() const {
// //         return voltage;
// //     }
// //
// //     // تنظیم ولتاژ گره
// //     void setVoltage(double v) {
// //         voltage = v;
// //     }
// //
// //     // بررسی اینکه آیا گره، گره زمین (گراند) است یا خیر
// //     bool isGround() const {
// //         return name == "0" || name == "GND" || name == "gnd"; // نام‌های رایج برای زمین
// //     }
// // };
// //
// // // تعریف کلاس پایه برای المان‌های مداری (Element)
// // class Element {
// // protected:
// //     Node *node1, *node2; // اشاره‌گر به گره‌های متصل به المان
// //     string name;         // نام المان
// //
// // public:
// //     // سازنده کلاس المان
// //     Element(Node* n1, Node* n2, const string &name) {
// //         if (!n1 || !n2) {
// //             throw std::invalid_argument("Nodes cannot be null.");
// //         }
// //         this->node1 = n1;
// //         this->node2 = n2;
// //         this->name = name;
// //     }
// //
// //     // مخرب مجازی پیش‌فرض
// //     virtual ~Element() = default;
// //
// //     // دریافت نام المان
// //     string getName() const { return name; }
// //
// //     // دریافت گره اول
// //     Node* getNode1() const { return node1; }
// //
// //     // دریافت گره دوم
// //     Node* getNode2() const { return node2; }
// //
// //     // تابع مجازی خالص برای دریافت نوع المان (باید در کلاس‌های مشتق پیاده‌سازی شود)
// //     virtual string getType() const = 0;
// //
// //     // تابع مجازی خالص برای دریافت مقدار مشخصه المان (مقاومت، ولتاژ منبع، جریان منبع و ...)
// //     virtual double getValue() const = 0;
// //
// //     // تابع مجازی برای دریافت جریان عبوری از المان (می‌تواند در کلاس‌های مشتق بازنویسی شود)
// //     // برای المان‌هایی مانند مقاومت پس از حل مدار محاسبه می‌شود
// //     // برای منابع ولتاژ، این یکی از مجهولات سیستم MNA است
// //     virtual double getCurrent() const {
// //         return 0.0; // پیاده‌سازی پیش‌فرض
// //     }
// //
// //     // تابع مجازی برای تنظیم جریان عبوری از المان (مربوط به منابع ولتاژ)
// //     virtual void setCurrent(double current) {
// //         // پیاده‌سازی پیش‌فرض، برای جلوگیری از هشدار پارامتر استفاده‌نشده
// //         (void)current;
// //     }
// // };
// //
// // // کلاس مقاومت (Resistor) مشتق‌شده از Element
// // class Resistor : public Element {
// // private:
// //     double resistance; // مقدار مقاومت
// //
// // public:
// //     Resistor(Node* n1, Node* n2, const string &name, double res) : Element(n1, n2, name) {
// //         if (res <= 0) {
// //             // مطابق فایل PDF، بخش ۱۹ (مدیریت خطاها)
// //             throw std::invalid_argument("Error:value of Resistor cannot be negative. Resistor Name: " + name);
// //         }
// //         this->resistance = res;
// //     }
// //
// //     string getType() const override { return "Resistor"; }
// //     double getValue() const override { return resistance; } // مقدار مقاومت را برمی‌گرداند
// //
// //     // جریان عبوری از مقاومت طبق قانون اهم (پس از مشخص شدن ولتاژ گره‌ها)
// //     double getCurrent() const override {
// //         if (node1 && node2) {
// //             return (node1->getVoltage() - node2->getVoltage()) / resistance;
// //         }
// //         return 0.0;
// //     }
// // };
// //
// // // کلاس منبع ولتاژ مستقل (VoltageSource) مشتق‌شده از Element
// // class VoltageSource : public Element {
// // private:
// //     double voltageValue;         // مقدار ولتاژ منبع
// //     double currentThroughSource; // جریان عبوری از منبع (یکی از مجهولات MNA)
// //
// // public:
// //     VoltageSource(Node* n1, Node* n2, const string &name, double val) : Element(n1, n2, name) {
// //         this->voltageValue = val;
// //         this->currentThroughSource = 0.0; // مقدار اولیه
// //     }
// //
// //     string getType() const override { return "VoltageSource"; }
// //     double getValue() const override { return voltageValue; } // مقدار ولتاژ تنظیم‌شده منبع را برمی‌گرداند
// //
// //     double getCurrent() const override {
// //         return currentThroughSource;
// //     }
// //     void setCurrent(double current) override {
// //         this->currentThroughSource = current;
// //     }
// // };
// //
// // // کلاس منبع جریان مستقل (CurrentSource) مشتق‌شده از Element
// // class CurrentSource : public Element {
// // private:
// //     double currentValue; // مقدار جریان منبع
// //
// // public:
// //     // گره n1 گرهی است که جریان از آن خارج می‌شود، گره n2 گرهی است که جریان به آن وارد می‌شود
// //     CurrentSource(Node* n1, Node* n2, const string &name, double val) : Element(n1, n2, name) {
// //         this->currentValue = val;
// //     }
// //
// //     string getType() const override { return "CurrentSource"; }
// //     double getValue() const override { return currentValue; } // مقدار جریان تنظیم‌شده منبع را برمی‌گرداند
// //
// //     // ولتاژ دو سر منبع جریان توسط مدار تعیین می‌شود و جزو مجهولات مستقیم برای این المان نیست
// // };
// //
// //
// // // کلاس مسئول تشکیل ماتریس‌های MNA
// // class MakingMNA {
// // private:
// //     vector<Node*> allNodesInCircuit;     // لیست تمام گره‌های موجود در مدار
// //     vector<Element*> elementsInCircuit;  // لیست تمام المان‌های موجود در مدار
// //     Node* groundNodeRef;                 // اشاره‌گر به گره زمین
// //
// //     // نگاشت‌ها برای تشکیل ماتریس
// //     map<Node*, int> nodeToIndexMap;          // نگاشت گره‌های غیرزمین به اندیس سطر/ستون در ماتریس MNA
// //     vector<Node*> orderedNonGroundNodes;     // لیست مرتب‌شده گره‌های غیرزمین
// //     map<VoltageSource*, int> vsToIndexMap;   // نگاشت منابع ولتاژ به اندیس مجهول جریانشان در MNA
// //     vector<VoltageSource*> orderedVoltageSources; // لیست مرتب‌شده منابع ولتاژ
// //
// //     // متد داخلی برای ساخت نگاشت‌ها
// //     void buildNodeAndVoltageSourceMaps() {
// //         nodeToIndexMap.clear();
// //         orderedNonGroundNodes.clear();
// //         vsToIndexMap.clear();
// //         orderedVoltageSources.clear();
// //
// //         // تعیین گره زمین در صورت عدم تنظیم صریح
// //         if (!groundNodeRef) {
// //             for (Node* n : allNodesInCircuit) {
// //                 if (n->isGround()) {
// //                     groundNodeRef = n;
// //                     break;
// //                 }
// //             }
// //             if (!groundNodeRef) {
// //                 // مطابق فایل PDF، بخش ۱۹ (مدیریت خطاها)
// //                 throw std::runtime_error("Error: invable Ground Node. impossiple solving!");
// //             }
// //         }
// //
// //         // شماره‌گذاری گره‌های غیرزمین
// //         int nodeIdx = 0;
// //         for (Node* node : allNodesInCircuit) {
// //             if (node != groundNodeRef) {
// //                 orderedNonGroundNodes.push_back(node);
// //                 nodeToIndexMap[node] = nodeIdx++;
// //             }
// //         }
// //
// //         // شماره‌گذاری منابع ولتاژ (برای مجهولات جریان آنها)
// //         int vsIdx = 0;
// //         for (Element* elem : elementsInCircuit) {
// //             if (auto vs = dynamic_cast<VoltageSource*>(elem)) {
// //                 orderedVoltageSources.push_back(vs);
// //                 vsToIndexMap[vs] = vsIdx++;
// //             }
// //         }
// //     }
// //
// // public:
// //     // سازنده
// //     MakingMNA() : groundNodeRef(nullptr) {}
// //
// //     // مخرب (در صورت نیاز به مدیریت حافظه پیچیده‌تر، باید تکمیل شود)
// //     ~MakingMNA() {
// //         // اگر گره‌ها و المان‌ها در این کلاس new شده باشند، باید delete شوند.
// //         // در این پیاده‌سازی، فرض بر این است که مدیریت حافظه آنها خارج از این کلاس انجام می‌شود.
// //     }
// //
// //     // افزودن گره به مدار
// //     void addNode(Node* node) {
// //         if (!node) return;
// //         allNodesInCircuit.push_back(node);
// //         if (node->isGround()) {
// //             if (groundNodeRef != nullptr && groundNodeRef != node) {
// //                 cout << "Warning: there is some Ground Node. first Node has been used: "
// //                      << groundNodeRef->getName() << endl;
// //             } else if (groundNodeRef == nullptr) {
// //                 groundNodeRef = node;
// //             }
// //         }
// //     }
// //
// //     // افزودن المان به مدار
// //     void addElement(Element* element) {
// //         if (!element) return;
// //         elementsInCircuit.push_back(element);
// //     }
// //
// //     // تنظیم صریح گره زمین
// //     void setGroundNode(Node* gnd) {
// //         if (!gnd) {
// //             throw std::invalid_argument("Ground Node cannot be null.");
// //         }
// //         bool found = false;
// //         for(Node* n : allNodesInCircuit) {
// //             if(n == gnd) {
// //                 found = true;
// //                 break;
// //             }
// //         }
// //         if(!found) addNode(gnd); // اگر گره زمین هنوز به لیست گره‌ها اضافه نشده، اضافه کن
// //
// //         groundNodeRef = gnd;
// //     }
// //
// //     // تشکیل و دریافت ماتریس سیستم A در معادله Ax = Z
// //     Matrix getSystemMatrixA() {
// //         buildNodeAndVoltageSourceMaps(); // اطمینان از به‌روز بودن نگاشت‌ها
// //
// //         int numNonGroundNodes = orderedNonGroundNodes.size();
// //         int numVoltageSources = orderedVoltageSources.size();
// //         int systemSize = numNonGroundNodes + numVoltageSources;
// //
// //         if (systemSize == 0) {
// //             // اگر مداری خالی یا فقط با یک گره زمین باشد
// //              throw std::runtime_error("Error: there is no eny Node.");
// //         }
// //
// //         Matrix A(systemSize, Vector(systemSize, 0.0));
// //
// //         // بخش G (مربوط به هدایت‌ها - مقاومت‌ها)
// //         for (Element* elem : elementsInCircuit) {
// //             if (auto res = dynamic_cast<Resistor*>(elem)) {
// //                 double conductance = 1.0 / res->getValue();
// //                 Node* n1 = res->getNode1();
// //                 Node* n2 = res->getNode2();
// //
// //                 // تاثیر بر قطر اصلی
// //                 if (n1 != groundNodeRef) {
// //                     A[nodeToIndexMap[n1]][nodeToIndexMap[n1]] += conductance;
// //                 }
// //                 if (n2 != groundNodeRef) {
// //                     A[nodeToIndexMap[n2]][nodeToIndexMap[n2]] += conductance;
// //                 }
// //                 // تاثیر بر عناصر خارج از قطر
// //                 if (n1 != groundNodeRef && n2 != groundNodeRef) {
// //                     A[nodeToIndexMap[n1]][nodeToIndexMap[n2]] -= conductance;
// //                     A[nodeToIndexMap[n2]][nodeToIndexMap[n1]] -= conductance;
// //                 }
// //             }
// //         }
// //
// //         // بخش B و C (مربوط به منابع ولتاژ)
// //         // طبق توضیحات فایل PDF بخش ۵.۲
// //         for (size_t i = 0; i < orderedVoltageSources.size(); ++i) {
// //             VoltageSource* vs = orderedVoltageSources[i];
// //             Node* n_plus = vs->getNode1();  // گره مثبت منبع ولتاژ
// //             Node* n_minus = vs->getNode2(); // گره منفی منبع ولتاژ
// //             int vsMNAIndex = vsToIndexMap[vs]; // اندیس مجهول جریان این منبع ولتاژ
// //
// //             if (n_plus != groundNodeRef) {
// //                 int nodeIdx = nodeToIndexMap[n_plus];
// //                 A[nodeIdx][numNonGroundNodes + vsMNAIndex] += 1.0;  // بخش B
// //                 A[numNonGroundNodes + vsMNAIndex][nodeIdx] += 1.0;  // بخش C
// //             }
// //             if (n_minus != groundNodeRef) {
// //                 int nodeIdx = nodeToIndexMap[n_minus];
// //                 A[nodeIdx][numNonGroundNodes + vsMNAIndex] -= 1.0; // بخش B
// //                 A[numNonGroundNodes + vsMNAIndex][nodeIdx] -= 1.0; // بخش C
// //             }
// //         }
// //
// //         // بخش D ماتریس A برای منابع ولتاژ مستقل ایده‌آل صفر است.
// //         // ماتریس با صفرها مقداردهی اولیه شده است.
// //
// //         return A;
// //     }
// //
// //     // تشکیل و دریافت بردار سمت راست Z (یا B در برخی منابع) در معادله Ax = Z
// //     Vector getSystemVectorZ() {
// //         // فرض می‌شود buildNodeAndVoltageSourceMaps() قبلاً فراخوانی شده است
// //         int numNonGroundNodes = orderedNonGroundNodes.size();
// //         int numVoltageSources = orderedVoltageSources.size();
// //         int systemSize = numNonGroundNodes + numVoltageSources;
// //
// //         if (systemSize == 0 && numNonGroundNodes == 0) {
// //              Vector Z_empty; // بردار خالی
// //              return Z_empty;
// //         }
// //
// //         Vector Z(systemSize, 0.0);
// //
// //         // بخش J (مربوط به منابع جریان)
// //         // طبق توضیحات فایل PDF بخش ۵.۲
// //         for (Element* elem : elementsInCircuit) {
// //             if (auto cs = dynamic_cast<CurrentSource*>(elem)) {
// //                 Node* n_from = cs->getNode1(); // جریان از این گره خارج می‌شود
// //                 Node* n_to = cs->getNode2();   // جریان به این گره وارد می‌شود
// //                 double currentValue = cs->getValue();
// //
// //                 if (n_to != groundNodeRef) { // جریان ورودی به گره n_to
// //                     Z[nodeToIndexMap[n_to]] += currentValue;
// //                 }
// //                 if (n_from != groundNodeRef) { // جریان خروجی از گره n_from
// //                     Z[nodeToIndexMap[n_from]] -= currentValue;
// //                 }
// //             }
// //         }
// //
// //         // بخش E (مربوط به مقادیر منابع ولتاژ)
// //         // طبق توضیحات فایل PDF بخش ۵.۲
// //         for (size_t i = 0; i < orderedVoltageSources.size(); ++i) {
// //             VoltageSource* vs = orderedVoltageSources[i];
// //             int vsMNAIndex = vsToIndexMap[vs]; // این اندیس برای vs در orderedVoltageSources است
// //             Z[numNonGroundNodes + vsMNAIndex] = vs->getValue();
// //         }
// //         return Z;
// //     }
// //
// //     // متدهای Getter برای دسترسی به لیست‌های داخلی (برای به‌روزرسانی وضعیت پس از حل)
// //     const vector<Node*>& getOrderedNonGroundNodes() const {
// //         return orderedNonGroundNodes;
// //     }
// //
// //     const vector<VoltageSource*>& getOrderedVoltageSources() const {
// //         return orderedVoltageSources;
// //     }
// //     // برای دسترسی به نگاشت vs->index برای بروزرسانی جریان vs ها
// //      const map<VoltageSource*, int>& getVoltageSourceToIndexMap() const {
// //         return vsToIndexMap;
// //     }
// //      const vector<Element*>& getAllElements() const { // برای محاسبه جریان مقاومت‌ها پس از حل
// //         return elementsInCircuit;
// //     }
// // };
// //
// // // کلاس حل‌کننده سیستم معادلات MNA
// // class MNASolver {
// // public:
// //     MNASolver() {}
// //
// //     // حل سیستم AX = Z و برگرداندن بردار X (شامل ولتاژ گره‌ها و سپس جریان منابع ولتاژ)
// //     // پیاده‌سازی با حذف گاوسی و محورگیری جزئی
// //     Vector solve(Matrix A, Vector Z) { // کپی از A و Z چون در حین عملیات تغییر می‌کنند
// //         int n = A.size();
// //         if (n == 0 || A[0].size() != static_cast<size_t>(n) || Z.size() != static_cast<size_t>(n)) {
// //             throw std::runtime_error("Error: solving problem.");
// //         }
// //
// //         const double EPS = 1e-9; // یک مقدار کوچک برای بررسی تکین بودن
// //
// //         for (int i = 0; i < n; ++i) {
// //             // محورگیری جزئی: یافتن بزرگترین عنصر در ستون فعلی (از سطر i به پایین)
// //             int pivot_row = i;
// //             for (int k = i + 1; k < n; ++k) {
// //                 if (std::abs(A[k][i]) > std::abs(A[pivot_row][i])) {
// //                     pivot_row = k;
// //                 }
// //             }
// //
// //             // جابجایی سطرها در A و Z
// //             if (pivot_row != i) {
// //                 std::swap(A[i], A[pivot_row]);
// //                 std::swap(Z[i], Z[pivot_row]);
// //             }
// //
// //             // بررسی تکین بودن (اگر عنصر روی قطر صفر یا خیلی کوچک باشد)
// //             if (std::abs(A[i][i]) < EPS) {
// //                 throw std::runtime_error("Error: solving problem for system.");
// //             }
// //
// //             // نرمال‌سازی سطر i (عنصر قطری را 1 کن)
// //             // این مرحله اختیاری است اما می‌تواند به پایداری کمک کند، گرچه در اینجا مستقیماً برای حذف استفاده می‌کنیم.
// //             // برای سادگی، مستقیماً از A[i][i] به عنوان مقسوم‌علیه استفاده می‌کنیم.
// //
// //             // حذف عناصر زیر قطر در ستون i
// //             for (int k = i + 1; k < n; ++k) {
// //                 double factor = A[k][i] / A[i][i];
// //                 for (int j = i; j < n; ++j) { // از ستون i شروع کن
// //                     A[k][j] -= factor * A[i][j];
// //                 }
// //                 Z[k] -= factor * Z[i];
// //             }
// //         }
// //
// //         // جایگزینی پسرو (Back Substitution)
// //         Vector X(n);
// //         for (int i = n - 1; i >= 0; --i) {
// //             double sum = 0.0;
// //             for (int j = i + 1; j < n; ++j) {
// //                 sum += A[i][j] * X[j];
// //             }
// //             if (std::abs(A[i][i]) < EPS) { // بررسی مجدد برای تقسیم بر صفر در جایگزینی پسرو
// //                  throw std::runtime_error("Error: solving problem for system.");
// //             }
// //             X[i] = (Z[i] - sum) / A[i][i];
// //         }
// //         return X;
// //     }
// //
// //
// //     // به‌روزرسانی ولتاژ گره‌ها و جریان منابع ولتاژ پس از حل سیستم
// //     void updateCircuitState(const Vector& X, MakingMNA& mnaCircuit) {
// //         const auto& nonGroundNodes = mnaCircuit.getOrderedNonGroundNodes();
// //         const auto& voltageSources = mnaCircuit.getOrderedVoltageSources();
// //         const auto& vsMap = mnaCircuit.getVoltageSourceToIndexMap();
// //
// //         int numNonGroundNodes = nonGroundNodes.size();
// //
// //         if (X.size() != numNonGroundNodes + voltageSources.size()) {
// //             throw std::runtime_error("Error: solving problem.");
// //         }
// //
// //         // به‌روزرسانی ولتاژ گره‌ها
// //         for (size_t i = 0; i < static_cast<size_t>(numNonGroundNodes); ++i) {
// //             nonGroundNodes[i]->setVoltage(X[i]);
// //         }
// //
// //         // به‌روزرسانی جریان عبوری از منابع ولتاژ
// //         for (size_t i = 0; i < voltageSources.size(); ++i) {
// //             VoltageSource* vs = voltageSources[i];
// //             // اندیس vs در بخش جریان‌های مجهول ماتریس MNA
// //             // vsMap کلیدش VoltageSource* و مقدارش اندیس اولیه است (0, 1, 2, ...)
// //             // این اندیس‌ها باید با ترتیب در orderedVoltageSources مطابقت داشته باشند.
// //             // بنابراین، اندیس در بردار X برابر است با numNonGroundNodes + (اندیس vs در orderedVoltageSources)
// //             // یا اگر vsMap اندیس صحیح را برای vs در X بدهد (یعنی vsMap[vs] اندیس نهایی در X باشد)
// //             // در پیاده‌سازی فعلی MakingMNA، vsMap اندیس‌های 0, 1, ... را برای vs ها برمی‌گرداند.
// //             int vsMNAIndex = vsMap.at(vs); // این اندیس 0, 1, ... برای منابع ولتاژ است
// //             vs->setCurrent(X[numNonGroundNodes + vsMNAIndex]);
// //         }
// //     }
// // };
// //
// //
// // // تابع کمکی برای چاپ ماتریس
// // void printMatrix(const Matrix& mat, const string& name) {
// //     cout << name << ":\n";
// //     if (mat.empty()) {
// //         cout << "(empty)\n";
// //         return;
// //     }
// //     for (const auto& row : mat) {
// //         for (double val : row) {
// //             cout << setw(10) << val << " ";
// //         }
// //         cout << endl;
// //     }
// //     cout << endl;
// // }
// //
// // // تابع کمکی برای چاپ بردار
// // void printVector(const Vector& vec, const string& name) {
// //     cout << name << ":\n";
// //     if (vec.empty()) {
// //         cout << "(empty)\n";
// //         return;
// //     }
// //     for (double val : vec) {
// //         cout << setw(10) << val << " ";
// //     }
// //     cout << endl << endl;
// // }
// //
// //
// // // تابع اصلی برای تست
// // int main() {
// //     // تنظیم دقت خروجی برای اعداد اعشاری
// //     cout << fixed << setprecision(6);
// //
// //     // ۱. ایجاد گره‌ها
// //     Node n1("1"), n2("2"), n_gnd("0"); // گره "0" یا "GND" معمولاً زمین است
// //
// //     // ۲. ایجاد مدیر مدار و افزودن گره‌ها
// //     MakingMNA circuit;
// //     circuit.addNode(&n1);
// //     circuit.addNode(&n2);
// //     circuit.addNode(&n_gnd);
// //     // circuit.setGroundNode(&n_gnd); // تنظیم صریح گره زمین (اگرچه به طور خودکار نیز شناسایی می‌شود)
// //
// //     // ۳. ایجاد المان‌ها و افزودن آنها به مدار
// //     try {
// //         // مثال از فایل PDF بخش ۹ (مدار RC): V1=5V, R=1k, C=1uF (اینجا C را با مقاومت دوم جایگزین می‌کنیم برای مثال DC)
// //         // V1 بین گره 1 و زمین، R1 بین 1 و 2، R2 بین 2 و زمین
// //         VoltageSource vs(&n1, &n_gnd, "V1", 5.0);   // منبع ولتاژ 5 ولت بین گره 1 و زمین
// //         Resistor r1(&n1, &n2, "R1", 1000.0);     // مقاومت 1 کیلواهم بین گره 1 و 2
// //         Resistor r2(&n2, &n_gnd, "R2", 2000.0);   // مقاومت 2 کیلواهم بین گره 2 و زمین
// //         // CurrentSource cs(&n_gnd, &n2, "I1", 0.001); // مثال: منبع جریان 1 میلی‌آمپر از زمین به گره 2
// //
// //         circuit.addElement(&vs);
// //         circuit.addElement(&r1);
// //         circuit.addElement(&r2);
// //         // circuit.addElement(&cs);
// //
// //         // ۴. دریافت ماتریس‌های MNA
// //         Matrix A = circuit.getSystemMatrixA();
// //         Vector Z = circuit.getSystemVectorZ();
// //
// //         printMatrix(A, "A Matrix");
// //         printVector(Z, "Z Matrix");
// //
// //
// //         // ۵. حل سیستم
// //         MNASolver solver;
// //         Vector X = solver.solve(A, Z); // A و Z به صورت کپی ارسال می‌شوند
// //
// //         printVector(X, "X vector");
// //
// //
// //         // ۶. به‌روزرسانی ولتاژ گره‌ها و جریان منابع ولتاژ در اشیاء مربوطه
// //         solver.updateCircuitState(X, circuit);
// //
// //         // ۷. چاپ نتایج
// //         cout << "Voltage of Nodes after solving: " << endl;
// //         for (const auto* node : circuit.getOrderedNonGroundNodes()) {
// //             cout << "Node " << node->getName() << ": " << node->getVoltage() << " V" << endl;
// //         }
// //
// //         cout << "\nCurrent Of voltage Source:" << endl;
// //         for (const auto* vs_elem : circuit.getOrderedVoltageSources()) {
// //             cout << "Current of " << vs_elem->getName() << ": " << vs_elem->getCurrent() << " A" << endl;
// //         }
// //
// //         cout << "\nCurrent of Resistors: " << endl;
// //         for (const auto* elem : circuit.getAllElements()) {
// //            if (const Resistor* res = dynamic_cast<const Resistor*>(elem)) {
// //                cout << "Current of " << res->getName() << " (" << res->getNode1()->getName() << "->" << res->getNode2()->getName() << "): "
// //                     << res->getCurrent() << " A" << endl;
// //            }
// //         }
// //
// //
// //     } catch (const std::exception& e) {
// //         cerr << "OOps. " << e.what() << endl;
// //     }
// //
// //     return 0;
// // }

// #include <iostream>
// #include <string>
// #include <vector>
// #include <regex>
// #include <stdexcept> // For logic_error
// #include <algorithm> // For tolower
// using namespace std;
//
// // Forward declaration of centralController
// class centralController {
// public:
//     // Methods for section 13 & 18 (element/source creation/deletion)
//     void makingResistor(const string& name, const string& node1, const string& node2, const string& value_str_raw) {
//         cout << "Controller: Making Resistor: " << name << ", " << node1 << ", " << node2 << ", Value: " << value_str_raw << endl;
//     }
//     void deletingResistor(const string& name) {
//         cout << "Controller: Deleting Resistor: " << name << endl;
//     }
//     void makingCapacity(const string& name, const string& node1, const string& node2, const string& value_str_raw) {
//         cout << "Controller: Making Capacitor: " << name << ", " << node1 << ", " << node2 << ", Value: " << value_str_raw << endl;
//     }
//     void deleteCapacity(const string& name) {
//         cout << "Controller: Deleting Capacitor: " << name << endl;
//     }
//     void makingInductor(const string& name, const string& node1, const string& node2, const string& value_str_raw) {
//         cout << "Controller: Making Inductor: " << name << ", " << node1 << ", " << node2 << ", Value: " << value_str_raw << endl;
//     }
//     void deleteInductor(const string& name) {
//         cout << "Controller: Deleting Inductor: " << name << endl;
//     }
//     void makingDiode(const string& name, const string& node1, const string& node2, const string& model) {
//         cout << "Controller: Making Diode: " << name << ", " << node1 << ", " << node2 << ", Model: " << model << endl;
//     }
//     void deletingDiode(const string& name) {
//         cout << "Controller: Deleting Diode: " << name << endl;
//     }
//     void addingGround(const string& node) {
//         cout << "Controller: Adding Ground to node: " << node << endl;
//     }
//     void deletingGround(const string& node) {
//         cout << "Controller: Deleting Ground from node: " << node << endl;
//     }
//
//     // --- Source Creation/Deletion ---
//     void makingVoltageSourceDC(const string& name, const string& node_plus, const string& node_minus, const string& value_str_raw) {
//         cout << "Controller: Making DC Voltage Source: " << name << ", N+: " << node_plus << ", N-: " << node_minus << ", Value: " << value_str_raw << endl;
//     }
//     void makingCurrentSourceDC(const string& name, const string& node_plus, const string& node_minus, const string& value_str_raw) {
//         cout << "Controller: Making DC Current Source: " << name << ", N+: " << node_plus << ", N-: " << node_minus << ", Value: " << value_str_raw << endl;
//     }
//     void makingVoltageSourceSin(const string& name, const string& node_plus, const string& node_minus, const string& v_offset, const string& v_amplitude, const string& freq) {
//         cout << "Controller: Making Sinusoidal Voltage Source: " << name << ", N+: " << node_plus << ", N-: " << node_minus
//              << ", Voffset: " << v_offset << ", Vamplitude: " << v_amplitude << ", Freq: " << freq << endl;
//     }
//     void makingVCVS(const string& name, const string& n_plus, const string& n_minus, const string& nc_plus, const string& nc_minus, const string& gain_str) {
//         cout << "Controller: Making VCVS (E): " << name << " " << n_plus << " " << n_minus << " " << nc_plus << " " << nc_minus << " " << gain_str << endl;
//     }
//     void makingVCCS(const string& name, const string& n_plus, const string& n_minus, const string& nc_plus, const string& nc_minus, const string& gain_str) {
//         cout << "Controller: Making VCCS (G): " << name << " " << n_plus << " " << n_minus << " " << nc_plus << " " << nc_minus << " " << gain_str << endl;
//     }
//     void makingCCVS(const string& name, const string& n_plus, const string& n_minus, const string& v_control_name, const string& gain_str) {
//         cout << "Controller: Making CCVS (H): " << name << " " << n_plus << " " << n_minus << " " << v_control_name << " " << gain_str << endl;
//     }
//     void makingCCCS(const string& name, const string& n_plus, const string& n_minus, const string& v_control_name, const string& gain_str) {
//         cout << "Controller: Making CCCS (F): " << name << " " << n_plus << " " << n_minus << " " << v_control_name << " " << gain_str << endl;
//     }
//     void deletingSource(const string& name) { // Generic delete for sources V, I, E, G, H, F
//         cout << "Controller: Deleting Source: " << name << endl;
//     }
//
//
//     // Placeholder methods for section 14, 15, 17 commands
//     void handleNodesCommand() {
//         cout << "Controller: Handling 'nodes' command." << endl;
//         cout << "Available nodes:\nn001, n002, VDD, GND, Vout, Vin" << endl; // PDF Example
//     }
//
//     void handleListCommand(const string& componentType = "") {
//         if (componentType.empty()) {
//             cout << "Controller: Handling 'list' command (all components)." << endl;
//         } else {
//             cout << "Controller: Handling 'list " << componentType << "' command." << endl;
//         }
//     }
//
//     void handleRenameNodeCommand(const string& oldName, const string& newName) {
//         // This method in controller would check for existence and then perform rename
//         cout << "Controller: Attempting to rename node " << oldName << " to " << newName << "." << endl;
//         // Simulate success based on PDF example
//         cout << "SUCCESS: Node renamed from " << oldName << " to " << newName << endl;
//         // Or throw specific errors based on checks:
//         // throw logic_error("ERROR: Node " + oldName + " does not exist in the circuit");
//         // throw logic_error("ERROR: Node name " + newName + " already exists");
//     }
//
//     void handlePrintCommand(const string& analysisType, const string& printArgs) {
//         cout << "Controller: Handling 'print " << analysisType << "' command with args: '" << printArgs << "'." << endl;
//         // Check for errors like "Node <node_name> not found in circuit" or "Component <component_name> not found" here
//     }
//
//     void handleNewFileCommand(const string& filePath) {
//         cout << "Controller: Handling 'NewFile " << filePath << "' command." << endl;
//     }
//
//     // --- Menu related state/handlers ---
//     bool is_expecting_schematic_choice = false; // Example state variable
//
//     void handleShowExistingSchematicsCommand() {
//         cout << "Controller: Handling 'show existing schematics' command." << endl;
//         cout << "-choose existing schematic:\n1-draft1\n2-draft2\n3-draft3\n4-elecphase1" << endl; // PDF Example
//         is_expecting_schematic_choice = true;
//     }
//
//     void handleChooseSchematic(const string& choice) {
//         is_expecting_schematic_choice = false; // Reset state
//         if (choice == "return") {
//             cout << "Controller: Returning to main menu..." << endl;
//         } else {
//             // Validate choice (e.g., is it a number within range?)
//             bool isValidChoice = true; // Placeholder for actual validation
//             for(char const &c : choice) { if (!std::isdigit(c)) isValidChoice = false; break;}
//             // Based on PDF, if choice is invalid (e.g., "25", "rreturn", "draft1")
//             if (!isValidChoice /* or choice out of bounds */ ) {
//                  cout << "-Error: Inappropriate input" << endl; // PDF Error message
//                  is_expecting_schematic_choice = true; // Remain in this state
//                  return;
//             }
//             cout << "Controller: Loading schematic " << choice << "..." << endl;
//             // ... display netlist ...
//             cout << "draft" << choice << ":\nV1 in 0 DC O AC 1 SIN(0 1 1k)\n...\n.end" << endl; // Simplified example
//             // According to PDF, after showing netlist, it returns to the "choose existing schematic" prompt
//             handleShowExistingSchematicsCommand();
//         }
//     }
// };
//
// // Helper functions (from user's .txt file, with minor improvements)
// bool checkDouble(const string& s) {
//     if (s.empty()) return false;
//     size_t i = 0;
//     bool negative = false;
//     if (s[0] == '-') {
//         if (s.length() == 1) return false; // Just "-"
//         negative = true;
//         i = 1;
//     }
//     bool digitFound = false;
//     bool dotFound = false;
//     for (; i < s.length(); ++i) {
//         if (isdigit(s[i])) {
//             digitFound = true;
//         } else if (s[i] == '.') {
//             if (dotFound) return false; // Multiple dots
//             dotFound = true;
//         } else {
//             return false; // Invalid character
//         }
//     }
//     if (!digitFound) return false; // No digits (e.g., ".", "-.")
//     try {
//         stod(s);
//     } catch (const std::invalid_argument&) {
//         return false;
//     } catch (const std::out_of_range&) {
//         return false;
//     }
//     return true;
// }
//
// bool checkingNemadElmi(const string& s) {
//     if (s.empty()) return false;
//     string temp_s = s;
//     std::transform(temp_s.begin(), temp_s.end(), temp_s.begin(), ::tolower); // Convert to lowercase for 'e'
//
//     size_t e_pos = temp_s.find('e');
//     if (e_pos == string::npos) return false; // No 'e'
//
//     string base_str = temp_s.substr(0, e_pos);
//     string exp_str = temp_s.substr(e_pos + 1);
//
//     if (base_str.empty() || exp_str.empty()) return false;
//
//     if (!checkDouble(base_str)) return false;
//
//     // Check exponent part (must be integer, possibly signed)
//     size_t i = 0;
//     if (exp_str[0] == '+' || exp_str[0] == '-') {
//         if (exp_str.length() == 1) return false; // "e+" or "e-"
//         i = 1;
//     }
//     if (i == exp_str.length()) return false; // "e" or "e+" with no digits after sign
//
//     for (; i < exp_str.length(); ++i) {
//         if (!isdigit(exp_str[i])) return false;
//     }
//     // Try converting the whole string to double to catch out_of_range for the combined value
//     try {
//         stod(s);
//     } catch (const std::out_of_range&) {
//         return false;
//     }
//     return true;
// }
//
// pair<string, string> valuate(const string& s_in) {
//     string s = s_in;
//     string value_str = s;
//     string prefix_str = "";
//
//     if (s.empty()) return {s, ""};
//
//     // Handle common SI prefixes. Order matters for multi-char prefixes (e.g., "Meg" before "m").
//     // Case-insensitive check for prefixes might be good, but PDF implies case sensitivity for values.
//     // For now, stick to what's common in SPICE-like inputs.
//     if (s.length() >= 3 && (s.substr(s.length() - 3) == "Meg" || s.substr(s.length() - 3) == "MEG")) {
//         value_str = s.substr(0, s.length() - 3);
//         prefix_str = "Meg";
//     } else if (s.length() >= 2) { // Check for single character prefixes
//         char last_char = s.back();
//         string potential_val_str = s.substr(0, s.length() - 1);
//         // Only treat as prefix if the remaining part is potentially a number
//         if (!potential_val_str.empty() && (isdigit(potential_val_str.back()) || potential_val_str.back() == '.')) {
//             switch (last_char) {
//                 case 'G': prefix_str = "G"; value_str = potential_val_str; break; // Giga
//                 // Meg handled above
//                 case 'k': case 'K': prefix_str = "k"; value_str = potential_val_str; break; // kilo
//                 case 'm': prefix_str = "m"; value_str = potential_val_str; break; // milli
//                 case 'u': case 'U': prefix_str = "u"; value_str = potential_val_str; break; // micro (often u in SPICE)
//                 case 'n': case 'N': prefix_str = "n"; value_str = potential_val_str; break; // nano
//                 case 'p': case 'P': prefix_str = "p"; value_str = potential_val_str; break; // pico
//                 case 'f': case 'F': prefix_str = "f"; value_str = potential_val_str; break; // femto
//                 // No default, if not a prefix, value_str remains s and prefix_str remains ""
//             }
//         }
//     }
//     // If value_str is now empty, it means the input was likely just a prefix (e.g., "k")
//     if (value_str.empty() && !prefix_str.empty()) {
//         // This is an invalid format, e.g. "k" alone. Let checkDouble/NemadElmi catch it.
//         return {s_in, ""}; // Return original and let validation fail
//     }
//     return {value_str, prefix_str};
// }
//
//
// class CommandParser {
// private:
//     centralController& controller;
//
//     // --- Element Parsing (Section 13) ---
//     bool tryParseElementCommands(const string& in) {
//         smatch matches;
//         // Resistor
//         regex add_res_pattern(R"(^add (R\w+) (\w+) (\w+) (.+?)$)"); // R<name> node1 node2 value
//         regex del_res_pattern(R"(^delete (R\w+)$)");
//         if (regex_match(in, matches, add_res_pattern)) {
//             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
//             pair<string, string> p_val = valuate(val_raw);
//             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Resistance cannot be zero or negative"); // PDF 13.1.5.1 (adapted)
//             if (stod(p_val.first) <= 0) throw logic_error("Error: Resistance cannot be zero or negative");
//             controller.makingResistor(name, n1, n2, val_raw); return true;
//         }
//         if (regex_match(in, matches, del_res_pattern)) {
//             controller.deletingResistor(matches[1].str()); return true;
//         }
//         // Capacitor
//         regex add_cap_pattern(R"(^add (C\w+) (\w+) (\w+) (.+?)$)");
//         regex del_cap_pattern(R"(^delete (C\w+)$)");
//         if (regex_match(in, matches, add_cap_pattern)) {
//             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
//             pair<string, string> p_val = valuate(val_raw);
//             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Capacitance cannot be zero or negative"); // PDF 13.2.5.1
//             if (stod(p_val.first) <= 0) throw logic_error("Error: Capacitance cannot be zero or negative");
//             controller.makingCapacity(name, n1, n2, val_raw); return true;
//         }
//         if (regex_match(in, matches, del_cap_pattern)) {
//             controller.deleteCapacity(matches[1].str()); return true;
//         }
//         // Inductor
//         regex add_ind_pattern(R"(^add (L\w+) (\w+) (\w+) (.+?)$)");
//         regex del_ind_pattern(R"(^delete (L\w+)$)");
//         if (regex_match(in, matches, add_ind_pattern)) {
//             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
//             pair<string, string> p_val = valuate(val_raw);
//             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Inductance cannot be zero or negative"); // PDF 13.3.5.1
//             if (stod(p_val.first) <= 0) throw logic_error("Error: Inductance cannot be zero or negative");
//             controller.makingInductor(name, n1, n2, val_raw); return true;
//         }
//         if (regex_match(in, matches, del_ind_pattern)) {
//             controller.deleteInductor(matches[1].str()); return true;
//         }
//         // Diode
//         regex add_diode_pattern(R"(^add (D\w+) (\w+) (\w+) (D|Z)$)"); // Model D or Z
//         regex del_diode_pattern(R"(^delete (D\w+)$)");
//         if (regex_match(in, matches, add_diode_pattern)) {
//             // Model validation is part of regex
//             controller.makingDiode(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
//         }
//         if (regex_match(in, matches, del_diode_pattern)) {
//             controller.deletingDiode(matches[1].str()); return true;
//         }
//         // Ground (PDF 13.5)
//         regex add_gnd_pattern(R"(^add GND (\w+)$)"); // add GND <node>
//         regex del_gnd_pattern(R"(^delete GND (\w+)$)"); // delete GND <node>
//         if (regex_match(in, matches, add_gnd_pattern)) {
//             controller.addingGround(matches[1].str()); return true;
//         }
//         if (regex_match(in, matches, del_gnd_pattern)) {
//             // PDF 13.5.2: "if node does not exist, print 'Node does not exist'" - This check is for controller
//             controller.deletingGround(matches[1].str()); return true;
//         }
//         return false;
//     }
//
//     // --- Source Parsing (Section 18) ---
//     bool tryParseSourceCommands(const string& in) {
//         smatch matches;
//         // Independent DC Voltage Source: add V<Name> <Node1> <Node2> <Value>
//         regex add_vdc_pattern(R"(^add (V\w+) (\w+) (\w+) ([^\s(]+)$)"); // Value should not start with ( for SIN/PULSE
//         if (regex_match(in, matches, add_vdc_pattern) && matches[4].str().find("SIN") == string::npos && matches[4].str().find("PULSE") == string::npos) {
//             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
//             pair<string, string> p_val = valuate(val_raw);
//             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value for DC voltage source " + name);
//             // DC value can be zero or negative
//             controller.makingVoltageSourceDC(name, n1, n2, val_raw); return true;
//         }
//         // Independent DC Current Source: add I<Name> <Node1> <Node2> <Value>
//         regex add_idc_pattern(R"(^add (I\w+) (\w+) (\w+) (.+?)$)");
//         if (regex_match(in, matches, add_idc_pattern)) {
//             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
//             pair<string, string> p_val = valuate(val_raw);
//             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value for DC current source " + name);
//             controller.makingCurrentSourceDC(name, n1, n2, val_raw); return true;
//         }
//         // Sinusoidal Voltage Source: add V<name> <n+> <n-> SIN(<Voffset> <Vamplitude> <Frequency>) (PDF 18.2)
//         // Regex needs to be careful with spaces around commas and parentheses.
//         regex add_vsin_pattern(R"(^add (V\w+) (\w+) (\w+) SIN\s*\(\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*\)$)");
//         if (regex_match(in, matches, add_vsin_pattern)) {
//             string name=matches[1].str(), n_plus=matches[2].str(), n_minus=matches[3].str();
//             string voff_raw=matches[4].str(), vamp_raw=matches[5].str(), freq_raw=matches[6].str();
//             // Validate voff, vamp, freq (should be numbers)
//             if (!checkDouble(valuate(voff_raw).first) && !checkingNemadElmi(valuate(voff_raw).first)) throw logic_error("Error: Invalid Voffset for SIN source " + name);
//             if (!checkDouble(valuate(vamp_raw).first) && !checkingNemadElmi(valuate(vamp_raw).first)) throw logic_error("Error: Invalid Vamplitude for SIN source " + name);
//             if (!checkDouble(valuate(freq_raw).first) && !checkingNemadElmi(valuate(freq_raw).first)) throw logic_error("Error: Invalid Frequency for SIN source " + name);
//             if (stod(valuate(freq_raw).first) <= 0) throw logic_error("Error: Frequency for SIN source " + name + " must be positive.");
//
//             controller.makingVoltageSourceSin(name, n_plus, n_minus, voff_raw, vamp_raw, freq_raw); return true;
//         }
//
//         // Dependent Sources (PDF 18.4)
//         // VCVS (E): add E<Name> <N1> <N2> <CtrlN1> <CtrlN2> <Gain>
//         regex add_vcvs_pattern(R"(^add (E\w+) (\w+) (\w+) (\w+) (\w+) (.+?)$)");
//         if (regex_match(in, matches, add_vcvs_pattern)) {
//             string gain_raw = matches[6].str();
//             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain for VCVS " + matches[1].str());
//             controller.makingVCVS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str(), gain_raw); return true;
//         }
//         // VCCS (G): add G<name> <n+> <n-> <control+> <control-> <Gain>
//         regex add_vccs_pattern(R"(^add (G\w+) (\w+) (\w+) (\w+) (\w+) (.+?)$)");
//         if (regex_match(in, matches, add_vccs_pattern)) {
//             string gain_raw = matches[6].str();
//             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain for VCCS " + matches[1].str());
//             controller.makingVCCS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str(), gain_raw); return true;
//         }
//         // CCVS (H): add H<name> <n+> <n-> <vname> <gain> (vname is name of a voltage source whose current is controlling)
//         regex add_ccvs_pattern(R"(^add (H\w+) (\w+) (\w+) (V\w+) (.+?)$)"); // Assuming vname starts with V
//         if (regex_match(in, matches, add_ccvs_pattern)) {
//             string gain_raw = matches[5].str();
//             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain for CCVS " + matches[1].str());
//             controller.makingCCVS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), gain_raw); return true;
//         }
//         // CCCS (F): add F<name> <n+> <n-> <vname> <gain>
//         regex add_cccs_pattern(R"(^add (F\w+) (\w+) (\w+) (V\w+) (.+?)$)"); // Assuming vname starts with V
//         if (regex_match(in, matches, add_cccs_pattern)) {
//             string gain_raw = matches[5].str();
//             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain for CCCS " + matches[1].str());
//             controller.makingCCCS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), gain_raw); return true;
//         }
//
//         // Generic delete for sources (V, I, E, G, H, F)
//         regex del_src_pattern(R"(^delete ([VIEGHF]\w+)$)");
//         if (regex_match(in, matches, del_src_pattern)) {
//             controller.deletingSource(matches[1].str()); return true;
//         }
//         return false;
//     }
//
//     // --- Node Operations (Section 14.1, 14.3) ---
//     bool tryParseNodeCommands(const string& in) {
//         smatch matches;
//         // nodes (PDF 14.1)
//         regex nodes_pattern(R"(^nodes$)");
//         if (regex_match(in, matches, nodes_pattern)) {
//             controller.handleNodesCommand(); return true;
//         }
//         // rename node <old_name> <new_name> (PDF 14.3)
//         regex rename_node_base_pattern(R"(^rename node)");
//         regex rename_node_full_pattern(R"(^rename node (\w+) (\w+)$)");
//         if (regex_match(in, matches, rename_node_full_pattern)) {
//             controller.handleRenameNodeCommand(matches[1].str(), matches[2].str()); return true;
//         } else if (regex_search(in, rename_node_base_pattern)) { // Starts with "rename node" but not full match
//             throw logic_error("ERROR: Invalid syntax correct format: rename node <old_name> <new_name>"); // PDF 14.3.7
//         }
//         return false;
//     }
//
//     // --- Listing Operations (Section 14.2) ---
//     bool tryParseListCommands(const string& in) {
//         smatch matches;
//         // list (PDF 14.2.1)
//         regex list_all_pattern(R"(^list$)");
//         if (regex_match(in, matches, list_all_pattern)) {
//             controller.handleListCommand(); return true;
//         }
//         // list [component_type] (PDF 14.2) - Assuming component_type is a single letter like R, C, L, V, I etc.
//         regex list_comp_pattern(R"(^list ([RCLDVIEGHF])$)"); // More specific component types
//         if (regex_match(in, matches, list_comp_pattern)) {
//             controller.handleListCommand(matches[1].str()); return true;
//         }
//         return false;
//     }
//
//     // --- Print/Output Operations (Section 15) ---
//     bool tryParsePrintCommands(const string& in) {
//         smatch matches;
//         // print <analysis_type> <variable1> ... (PDF 15.1)
//         // Example: print TRAN <Tstep> <Tstop> [<Tstart>] [<Tmaxstep>] V(n001) (PDF 15.2)
//         // Example: print DC <SrcName> <Start> <End> <Inc> V(out) (PDF 15.2)
//         // This regex is generic; detailed parsing of args is for controller or analysis module
//         regex print_pattern(R"(^print (TRAN|DC|AC) (.+)$)"); // AC added as typical
//         if (regex_match(in, matches, print_pattern)) {
//             try {
//                 controller.handlePrintCommand(matches[1].str(), matches[2].str());
//             } catch (const logic_error& e) { // Catch errors from controller like "Node not found"
//                  // Check if the error message from controller matches PDF spec for print errors
//                 string err_msg = e.what();
//                 if (err_msg.find("not found in circuit") != string::npos) { // PDF 15.5.1, 15.5.2
//                     throw; // Re-throw the specific error from controller
//                 }
//                 throw logic_error("Syntax error in command"); // PDF 15.5.3 generic syntax error
//             }
//             return true;
//         }
//         return false;
//     }
//
//     // --- File and Menu Operations (Section 16, 17) ---
//     bool tryParseFileAndMenuCommands(const string& in) {
//         smatch matches;
//         // NewFile <file_path> (PDF 17, after schematic list)
//         regex new_file_pattern(R"(^NewFile\s+(.+)$)");
//         if (regex_match(in, matches, new_file_pattern)) {
//             controller.handleNewFileCommand(matches[1].str()); return true;
//         }
//         // show existing schematics (PDF 17)
//         regex show_schematics_pattern(R"(^show existing schematics$)");
//         if (regex_match(in, matches, show_schematics_pattern)) {
//             controller.handleShowExistingSchematicsCommand(); return true;
//         }
//
//         // Context-dependent input for choosing schematic (PDF 17)
//         // This part is tricky as it depends on controller state
//         if (controller.is_expecting_schematic_choice) {
//             regex choose_num_pattern(R"(^(\d+)$)");
//             regex choose_return_pattern(R"(^return$)");
//             // Also handle invalid inputs like "draft1" directly as per PDF example
//             regex choose_text_pattern(R"(^[a-zA-Z]+.*$)");
//
//
//             if (regex_match(in, matches, choose_num_pattern)) {
//                 controller.handleChooseSchematic(matches[1].str()); return true;
//             } else if (regex_match(in, matches, choose_return_pattern)) {
//                 controller.handleChooseSchematic("return"); return true;
//             } else if (regex_match(in, matches, choose_text_pattern) || !in.empty()) {
//                 // If it's not a number or "return", but something was typed (e.g. "draft1", "rreturn", "25")
//                 // The controller's handleChooseSchematic should emit "-Error: Inappropriate input"
//                 // and keep is_expecting_schematic_choice = true
//                 controller.handleChooseSchematic(in); // Pass the raw invalid input
//                 return true;
//             }
//         }
//         return false;
//     }
//
//
// public:
//     explicit CommandParser(centralController& ctrl) : controller(ctrl) {}
//
//     void processInput(const string& in) {
//         if (in.empty()) return; // Ignore empty input
//
//         // Order of parsing attempts matters. More specific or context-dependent first.
//         if (tryParseFileAndMenuCommands(in)) return; // Handles menu states
//         if (tryParseNodeCommands(in)) return;
//         if (tryParseListCommands(in)) return;
//         if (tryParsePrintCommands(in)) return;
//         if (tryParseElementCommands(in)) return;
//         if (tryParseSourceCommands(in)) return; // Sources are also like elements
//
//         // If no command matched:
//         // Check for common syntax errors for commands that might have been partially typed
//         // e.g., "add" without enough arguments, or "delete" without enough.
//         // The PDF provides specific syntax error messages for some commands (like rename node).
//         // For others, a generic "Syntax error" or "Unknown command" might be appropriate.
//         // The current structure throws specific errors from within tryParse methods if a base pattern matches but details are wrong.
//
//         // Final fallback for completely unrecognized commands
//         // The PDF uses "Error: Syntax error" for some malformed element definitions (e.g. 13.1.5.2)
//         // and "Syntax error in command" for print (15.5.3).
//         // Let's use a general one.
//         throw logic_error("Error: Unknown command or invalid syntax. -> " + in);
//     }
// };
//
// int main() {
//     centralController controller_instance;
//     CommandParser parser(controller_instance);
//
//     vector<string> test_commands = {
//         // Section 14
//         "nodes",
//         "list",
//         "list R",
//         "list V",
//         "rename node N001 Vout",
//         "rename node N002", // Syntax Error
//         // Section 15
//         "print TRAN V(n001) I(R1)",
//         "print DC Vsource 0 5 0.1 V(out)",
//         // Section 17
//         "show existing schematics", // Controller sets state
//         "1",                      // To be handled by controller based on state
//         "return",                 // To be handled by controller based on state
//         "draft1",                 // Invalid choice, controller should show error
//         "NewFile /path/to/circuit.txt",
//         // Section 13 Elements
//         "add R1 N1 N2 1k",
//         "add Rload N_in N_out 2.2Meg",
//         "add C1 N2 0 10u",
//         "add L1 N1 N_intermediate 1mH",
//         "add D1 N_intermediate N2 D",
//         "add D_zener Vcc Vout Z",
//         "delete R1",
//         "add GND 0",
//         "delete GND 0",
//         "add RX N1 N2 0",     // Invalid value (caught by check)
//         "add R2 N3 N4 -100",  // Invalid value (caught by check)
//         "add R3 N5 N6 10p",
//         "add R4 N7 N8 1.2e-3",
//         "add R5 N9 N10 1.2e", // Invalid nemadElmi
//         // Section 18 Sources
//         "add Vsrc N_plus N_minus 5",
//         "add Isrc N_in N_out 100m",
//         "add Vosc P1 P2 SIN(0, 5, 1k)", // Test spaces in SIN
//         "add Vpulse P3 P4 SIN( 1, 2.5, 500 )",
//         "add Eamp Nout Nin Nctrl1 Nctrl2 100",
//         "add Gtrans N_curr_out N_curr_in N_volt_ctrl1 N_volt_ctrl2 0.1",
//         "add Hsense N_volt_out N_volt_gnd Vcontrol_current_src 50",
//         "add Fmirror N_curr_out_2 N_curr_gnd_2 Vcontrol_current_src_for_F 1",
//         "delete Vsrc",
//         // Errors
//         "add Rtest N1 N2 value_without_prefix", // Should be caught by checkDouble/NemadElmi
//         "add Ctest N1 N2 100pF", // valuate should handle 'pF' if 'F' is added
//         "add Ltest N1 N2 k", // valuate should return "k", then checkDouble fails
//         "unknown command here",
//         "add", // Incomplete
//         "delete" // Incomplete
//     };
//
//     cout << "--- Starting Command Tests ---" << endl;
//     for (const string& cmd : test_commands) {
//         cout << "\nInput: \"" << cmd << "\"" << endl;
//         try {
//             parser.processInput(cmd);
//         } catch (const logic_error& e) {
//             cerr << "Exception: " << e.what() << endl;
//         }
//     }
//     cout << "\n--- Testing Schematic Choice Menu ---" << endl;
//     string menu_inputs[] = {"show existing schematics", "5", "draft2", "1", "return"};
//     for (const string& cmd : menu_inputs) {
//         cout << "\nInput: \"" << cmd << "\"" << endl;
//         try {
//             parser.processInput(cmd);
//         } catch (const logic_error& e) {
//             cerr << "Exception: " << e.what() << endl;
//         }
//     }
//
//     return 0;
//}

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <stdexcept> // For logic_error
#include <algorithm> // For tolower, transform
#include <iomanip>   // For stod precision issues if any, though not directly used for that here

using namespace std;

// Forward declaration of centralController
class centralController {
public:
    // Methods for section 13 & 18 (element/source creation/deletion)
    void makingResistor(const string& name, const string& node1, const string& node2, const string& value_str_raw) {
        cout << "Controller: Making Resistor: " << name << ", " << node1 << ", " << node2 << ", Value: " << value_str_raw << endl;
    }
    void deletingResistor(const string& name) {
        cout << "Controller: Deleting Resistor: " << name << endl;
    }
    void makingCapacity(const string& name, const string& node1, const string& node2, const string& value_str_raw) {
        cout << "Controller: Making Capacitor: " << name << ", " << node1 << ", " << node2 << ", Value: " << value_str_raw << endl;
    }
    void deleteCapacity(const string& name) {
        cout << "Controller: Deleting Capacitor: " << name << endl;
    }
    void makingInductor(const string& name, const string& node1, const string& node2, const string& value_str_raw) {
        cout << "Controller: Making Inductor: " << name << ", " << node1 << ", " << node2 << ", Value: " << value_str_raw << endl;
    }
    void deleteInductor(const string& name) {
        cout << "Controller: Deleting Inductor: " << name << endl;
    }
    void makingDiode(const string& name, const string& node1, const string& node2, const string& model) {
        cout << "Controller: Making Diode: " << name << ", " << node1 << ", " << node2 << ", Model: " << model << endl;
    }
    void deletingDiode(const string& name) {
        cout << "Controller: Deleting Diode: " << name << endl;
    }
    void addingGround(const string& node) {
        cout << "Controller: Adding Ground to node: " << node << endl;
    }
    void deletingGround(const string& node) {
        // As per PDF 13.5.2, this method in controller should check if node exists
        // and print "Node does not exist" if applicable.
        cout << "Controller: Deleting Ground from node: " << node << endl;
    }

    // --- Source Creation/Deletion ---
    void makingVoltageSourceDC(const string& name, const string& node_plus, const string& node_minus, const string& value_str_raw) {
        cout << "Controller: Making DC Voltage Source: " << name << ", N+: " << node_plus << ", N-: " << node_minus << ", Value: " << value_str_raw << endl;
    }
    void makingCurrentSourceDC(const string& name, const string& node_plus, const string& node_minus, const string& value_str_raw) {
        cout << "Controller: Making DC Current Source: " << name << ", N+: " << node_plus << ", N-: " << node_minus << ", Value: " << value_str_raw << endl;
    }
    void makingVoltageSourceSin(const string& name, const string& node_plus, const string& node_minus, const string& v_offset, const string& v_amplitude, const string& freq) {
        cout << "Controller: Making Sinusoidal Voltage Source: " << name << ", N+: " << node_plus << ", N-: " << node_minus
             << ", Voffset: " << v_offset << ", Vamplitude: " << v_amplitude << ", Freq: " << freq << endl;
    }
    void makingVCVS(const string& name, const string& n_plus, const string& n_minus, const string& nc_plus, const string& nc_minus, const string& gain_str) {
        cout << "Controller: Making VCVS (E): " << name << " " << n_plus << " " << n_minus << " " << nc_plus << " " << nc_minus << " " << gain_str << endl;
    }
    void makingVCCS(const string& name, const string& n_plus, const string& n_minus, const string& nc_plus, const string& nc_minus, const string& gain_str) {
        cout << "Controller: Making VCCS (G): " << name << " " << n_plus << " " << n_minus << " " << nc_plus << " " << nc_minus << " " << gain_str << endl;
    }
    void makingCCVS(const string& name, const string& n_plus, const string& n_minus, const string& v_control_name, const string& gain_str) {
        cout << "Controller: Making CCVS (H): " << name << " " << n_plus << " " << n_minus << " " << v_control_name << " " << gain_str << endl;
    }
    void makingCCCS(const string& name, const string& n_plus, const string& n_minus, const string& v_control_name, const string& gain_str) {
        cout << "Controller: Making CCCS (F): " << name << " " << n_plus << " " << n_minus << " " << v_control_name << " " << gain_str << endl;
    }
    void deletingSource(const string& name) {
        cout << "Controller: Deleting Source: " << name << endl;
    }


    void handleNodesCommand() {
        cout << "Controller: Handling 'nodes' command." << endl;
        cout << "Available nodes:\nn001, n002, VDD, GND, Vout, Vin" << endl;
    }

    void handleListCommand(const string& componentType = "") {
        if (componentType.empty()) {
            cout << "Controller: Handling 'list' command (all components)." << endl;
        } else {
            cout << "Controller: Handling 'list " << componentType << "' command." << endl;
        }
    }

    void handleRenameNodeCommand(const string& oldName, const string& newName) {
        cout << "Controller: Attempting to rename node " << oldName << " to " << newName << "." << endl;
        // Simulate success based on PDF example for "rename node N001 Vout" (14.3.2)
        // Actual controller would check:
        // 1. if oldName exists (PDF 14.3.5: ERROR: Node <old_name> does not exist)
        // 2. if newName already exists (PDF 14.3.6: ERROR: Node name <new_name> already exists)
        cout << "SUCCESS: Node renamed from " << oldName << " to " << newName << endl;
    }

    void handlePrintCommand(const string& analysisType, const string& printArgs) {
        cout << "Controller: Handling 'print " << analysisType << "' command with args: '" << printArgs << "'." << endl;
        // Controller would parse printArgs and check for errors like:
        // PDF 15.5.1: Node <node_name> not found in circuit
        // PDF 15.5.2: Component <component_name> not found in circuit
    }

    void handleNewFileCommand(const string& filePath) {
        cout << "Controller: Handling 'NewFile " << filePath << "' command." << endl;
    }

    bool is_expecting_schematic_choice = false;

    void handleShowExistingSchematicsCommand() {
        cout << "Controller: Handling 'show existing schematics' command." << endl;
        cout << "-choose existing schematic:\n1-draft1\n2-draft2\n3-draft3\n4-elecphase1" << endl;
        is_expecting_schematic_choice = true;
    }

    void handleChooseSchematic(const string& choice) {
        // This method is responsible for PDF 17's menu logic including error "-Error: Inappropriate input"
        cout << "Controller: Handling schematic choice: " << choice << endl;

        if (choice == "return") {
            is_expecting_schematic_choice = false;
            cout << "Controller: Returning to main menu..." << endl;
            return;
        }

        bool isValidNumericChoice = true;
        int chosen_num = 0;
        if (choice.empty() || !all_of(choice.begin(), choice.end(), ::isdigit)) {
            isValidNumericChoice = false;
        } else {
            try {
                chosen_num = stoi(choice);
            } catch (const std::out_of_range&) {
                isValidNumericChoice = false; // Number too large
            }
        }

        // Example valid range based on PDF output
        if (isValidNumericChoice && chosen_num >= 1 && chosen_num <= 4) {
            is_expecting_schematic_choice = false; // Valid choice, reset state for now
            cout << "Controller: Loading schematic " << choice << "..." << endl;
            cout << "draft" << choice << ":\nV1 in 0 DC O AC 1 SIN(0 1 1k)\n...\n.end" << endl;
            // As per PDF, after showing netlist, it returns to the "choose existing schematic" prompt.
            handleShowExistingSchematicsCommand(); // This will set is_expecting_schematic_choice back to true
        } else {
            // Invalid choice (e.g., "5", "draft2", "rreturn" as per PDF examples of bad input)
            cout << "-Error: Inappropriate input" << endl; // PDF Error message (section 17)
            // is_expecting_schematic_choice remains true, so it prompts again implicitly
        }
    }
};

// Helper functions
bool checkDouble(const string& s_in) {
    if (s_in.empty()) return false;
    string s = s_in; // Make a copy for potential modification if needed (not currently)
    size_t i = 0;
    if (s[0] == '-') {
        if (s.length() == 1) return false;
        i = 1;
    }
    bool digitFound = false;
    bool dotFound = false;
    bool hasCharAfterDot = false;

    for (; i < s.length(); ++i) {
        if (isdigit(s[i])) {
            digitFound = true;
            if (dotFound) hasCharAfterDot = true;
        } else if (s[i] == '.') {
            if (dotFound) return false;
            dotFound = true;
        } else {
            return false;
        }
    }
    if (!digitFound) return false; // e.g., "." or "-."
    if (dotFound && !hasCharAfterDot && s.length() > 0 && s.back() == '.') {
        // handles cases like "1." or "-2." which stod might accept but we might want to reject if strict
        // For now, let stod decide. If stod accepts "1.", it's fine.
    }
    try {
        stod(s); // Check if convertible
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
    return true;
}

bool checkingNemadElmi(const string& s_in) {
    if (s_in.empty()) return false;
    string s = s_in;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);

    size_t e_pos = s.find('e');
    if (e_pos == string::npos || e_pos == 0 || e_pos == s.length() - 1) return false;

    string base_str = s.substr(0, e_pos);
    string exp_str = s.substr(e_pos + 1);

    if (base_str.empty() || exp_str.empty()) return false;
    if (!checkDouble(base_str)) return false; // Base must be a valid double

    size_t i = 0;
    if (exp_str[0] == '+' || exp_str[0] == '-') {
        if (exp_str.length() == 1) return false;
        i = 1;
    }
    if (i == exp_str.length()) return false;

    for (; i < exp_str.length(); ++i) {
        if (!isdigit(exp_str[i])) return false;
    }
    try {
        stod(s_in); // Check the original string for overall range
    } catch (const std::out_of_range&) {
        return false;
    }
    return true;
}

pair<string, string> valuate(const string& s_in) {
    string s = s_in;
    string value_str = s;
    string prefix_str = "";

    if (s.empty()) return {s, ""};

    // Prefixes from PDF: k, Meg, u, n, m. Also common G, p, f.
    // Order matters for multi-char prefixes (e.g., "Meg" before "m").
    if (s.length() >= 3 && (s.substr(s.length() - 3) == "Meg" || s.substr(s.length() - 3) == "MEG")) {
        value_str = s.substr(0, s.length() - 3);
        prefix_str = "Meg";
    } else if (s.length() >= 2) {
        char last_char = s.back();
        string potential_val_str = s.substr(0, s.length() - 1);
        if (!potential_val_str.empty() && (isdigit(potential_val_str.back()) || potential_val_str.back() == '.' || (potential_val_str.length() == 1 && potential_val_str[0] == '-'))) {
            switch (last_char) {
                case 'G': case 'g': prefix_str = "G"; value_str = potential_val_str; break;
                case 'k': case 'K': prefix_str = "k"; value_str = potential_val_str; break;
                case 'm': prefix_str = "m"; value_str = potential_val_str; break; // milli
                case 'u': case 'U': prefix_str = "u"; value_str = potential_val_str; break;
                case 'n': case 'N': prefix_str = "n"; value_str = potential_val_str; break;
                case 'p': case 'P': prefix_str = "p"; value_str = potential_val_str; break;
                case 'f': case 'F': prefix_str = "f"; value_str = potential_val_str; break;
                // Note: 'F' for femto, not Farad. PDF uses '1u', not '1uF'.
            }
        }
    }
    if (value_str.empty() && !prefix_str.empty()) {
        return {s_in, ""};
    }
    // The PDF does not show units like 'H', 'F', 'Ohm' appended to the value string.
    // e.g., it's "1m", not "1mH". So, we don't strip those here.
    // If "1mH" is passed, value_str will be "1mH", which will fail checkDouble/NemadElmi.
    return {value_str, prefix_str};
}


class CommandParser {
private:
    centralController& controller;

    // --- Element Parsing (Section 13) ---
    bool tryParseElementCommands(const string& in) {
        smatch matches;
        // Resistor
        regex add_res_pattern(R"(^add (R\w*) (\w+) (\w+) (.+?)$)");
        regex del_res_pattern(R"(^delete (R\w+)$)");
        if (regex_match(in, matches, add_res_pattern)) {
            string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
            pair<string, string> p_val = valuate(val_raw);
            if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for " + name);
            double num_val = stod(p_val.first);
            if (num_val <= 0) throw logic_error("Error: Resistance cannot be zero or negative"); // PDF 13.1.5.1
            controller.makingResistor(name, n1, n2, val_raw); return true;
        }
        if (regex_match(in, matches, del_res_pattern)) {
            controller.deletingResistor(matches[1].str()); return true;
        }
        // Capacitor
        regex add_cap_pattern(R"(^add (C\w+) (\w+) (\w+) (.+?)$)");
        regex del_cap_pattern(R"(^delete (C\w+)$)");
        if (regex_match(in, matches, add_cap_pattern)) {
            string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
            pair<string, string> p_val = valuate(val_raw);
            if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for " + name);
            double num_val = stod(p_val.first);
            if (num_val <= 0) throw logic_error("Error: Capacitance cannot be zero or negative"); // PDF 13.2.5.1
            controller.makingCapacity(name, n1, n2, val_raw); return true;
        }
        if (regex_match(in, matches, del_cap_pattern)) {
            controller.deleteCapacity(matches[1].str()); return true;
        }
        // Inductor
        regex add_ind_pattern(R"(^add (L\w+) (\w+) (\w+) (.+?)$)");
        regex del_ind_pattern(R"(^delete (L\w+)$)");
        if (regex_match(in, matches, add_ind_pattern)) {
            string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
            pair<string, string> p_val = valuate(val_raw);
            // If val_raw is "1mH", p_val.first will be "1mH". checkDouble("1mH") is false.
            if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for " + name);
            double num_val = stod(p_val.first);
            if (num_val <= 0) throw logic_error("Error: Inductance cannot be zero or negative"); // PDF 13.3.5.1
            controller.makingInductor(name, n1, n2, val_raw); return true;
        }
        if (regex_match(in, matches, del_ind_pattern)) {
            controller.deleteInductor(matches[1].str()); return true;
        }
        // Diode
        regex add_diode_pattern(R"(^add (D\w+) (\w+) (\w+) (D|Z)$)");
        regex del_diode_pattern(R"(^delete (D\w+)$)");
        if (regex_match(in, matches, add_diode_pattern)) {
            controller.makingDiode(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
        }
        if (regex_match(in, matches, del_diode_pattern)) {
            controller.deletingDiode(matches[1].str()); return true;
        }
        // Ground (PDF 13.5)
        regex add_gnd_pattern(R"(^add GND (\w+)$)");
        regex del_gnd_pattern(R"(^delete GND (\w+)$)");
        if (regex_match(in, matches, add_gnd_pattern)) {
            controller.addingGround(matches[1].str()); return true;
        }
        if (regex_match(in, matches, del_gnd_pattern)) {
            controller.deletingGround(matches[1].str()); return true;
        }
        return false;
    }

    // --- Source Parsing (Section 18) ---
    bool tryParseSourceCommands(const string& in) {
        smatch matches;
        // Independent DC Voltage Source: add V<Name> <Node1> <Node2> <Value>
        // Value should not be SIN(...) or PULSE(...)
        regex add_vdc_pattern(R"(^add (V\w+) (\w+) (\w+) ([^S\s][^I\s][^N\s][^P\s][^U\s][^L\s][^S\s][^E\s].*?|[^-+0-9.]?\S+)$)");
        // A more robust way for VDC value: capture everything and check if it's NOT SIN/PULSE and IS a number.
        regex add_v_generic_pattern(R"(^add (V\w+) (\w+) (\w+) (.+?)$)");

        if (regex_match(in, matches, add_v_generic_pattern)) {
            string name = matches[1].str();
            string n1 = matches[2].str();
            string n2 = matches[3].str();
            string val_full_str = matches[4].str();

            // Check for SIN
            regex vsin_params_pattern(R"(^SIN\s*\(\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*\)$)");
            smatch sin_matches;
            if (regex_match(val_full_str, sin_matches, vsin_params_pattern)) {
                string voff_raw=sin_matches[1].str(), vamp_raw=sin_matches[2].str(), freq_raw=sin_matches[3].str();
                if (!checkDouble(valuate(voff_raw).first) && !checkingNemadElmi(valuate(voff_raw).first)) throw logic_error("Error: Invalid Voffset for SIN source " + name);
                if (!checkDouble(valuate(vamp_raw).first) && !checkingNemadElmi(valuate(vamp_raw).first)) throw logic_error("Error: Invalid Vamplitude for SIN source " + name);
                if (!checkDouble(valuate(freq_raw).first) && !checkingNemadElmi(valuate(freq_raw).first)) throw logic_error("Error: Invalid Frequency for SIN source " + name);
                if (stod(valuate(freq_raw).first) <= 0) throw logic_error("Error: Frequency for SIN source " + name + " must be positive.");
                controller.makingVoltageSourceSin(name, n1, n2, voff_raw, vamp_raw, freq_raw); return true;
            }
            // Add PULSE check here if implementing
            // else if (is PULSE) { ... }

            // If not SIN or PULSE, assume DC
            else {
                pair<string, string> p_val = valuate(val_full_str);
                if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for DC voltage source " + name);
                // DC value can be zero or negative, so no <=0 check here.
                controller.makingVoltageSourceDC(name, n1, n2, val_full_str); return true;
            }
        }

        // Independent DC Current Source: add I<Name> <Node1> <Node2> <Value>
        regex add_idc_pattern(R"(^add (I\w+) (\w+) (\w+) (.+?)$)");
        if (regex_match(in, matches, add_idc_pattern)) {
            string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
            pair<string, string> p_val = valuate(val_raw);
            if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for DC current source " + name);
            controller.makingCurrentSourceDC(name, n1, n2, val_raw); return true;
        }

        // Dependent Sources (PDF 18.4)
        regex add_vcvs_pattern(R"(^add (E\w+) (\w+) (\w+) (\w+) (\w+) (.+?)$)");
        if (regex_match(in, matches, add_vcvs_pattern)) {
            string name=matches[1].str(), n_p=matches[2].str(), n_m=matches[3].str(), nc_p=matches[4].str(), nc_m=matches[5].str(), gain_raw=matches[6].str();
            if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain format for VCVS " + name);
            controller.makingVCVS(name, n_p, n_m, nc_p, nc_m, gain_raw); return true;
        }
        regex add_vccs_pattern(R"(^add (G\w+) (\w+) (\w+) (\w+) (\w+) (.+?)$)");
        if (regex_match(in, matches, add_vccs_pattern)) {
            string name=matches[1].str(), n_p=matches[2].str(), n_m=matches[3].str(), nc_p=matches[4].str(), nc_m=matches[5].str(), gain_raw=matches[6].str();
            if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain format for VCCS " + name);
            controller.makingVCCS(name, n_p, n_m, nc_p, nc_m, gain_raw); return true;
        }
        regex add_ccvs_pattern(R"(^add (H\w+) (\w+) (\w+) (V\w+) (.+?)$)");
        if (regex_match(in, matches, add_ccvs_pattern)) {
            string name=matches[1].str(), n_p=matches[2].str(), n_m=matches[3].str(), vctrl_name=matches[4].str(), gain_raw=matches[5].str();
            if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain format for CCVS " + name);
            controller.makingCCVS(name, n_p, n_m, vctrl_name, gain_raw); return true;
        }
        regex add_cccs_pattern(R"(^add (F\w+) (\w+) (\w+) (V\w+) (.+?)$)");
        if (regex_match(in, matches, add_cccs_pattern)) {
            string name=matches[1].str(), n_p=matches[2].str(), n_m=matches[3].str(), vctrl_name=matches[4].str(), gain_raw=matches[5].str();
            if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain format for CCCS " + name);
            controller.makingCCCS(name, n_p, n_m, vctrl_name, gain_raw); return true;
        }

        regex del_src_pattern(R"(^delete ([VIEGHF]\w+)$)");
        if (regex_match(in, matches, del_src_pattern)) {
            controller.deletingSource(matches[1].str()); return true;
        }
        return false;
    }

    bool tryParseNodeCommands(const string& in) {
        smatch matches;
        regex nodes_pattern(R"(^nodes$)");
        if (regex_match(in, matches, nodes_pattern)) {
            controller.handleNodesCommand(); return true;
        }
        regex rename_node_base_pattern(R"(^rename node)"); // For catching syntax error
        regex rename_node_full_pattern(R"(^rename node (\w+) (\w+)$)");
        if (regex_match(in, matches, rename_node_full_pattern)) {
            controller.handleRenameNodeCommand(matches[1].str(), matches[2].str()); return true;
        } else if (regex_search(in, rename_node_base_pattern)) {
            throw logic_error("ERROR: Invalid syntax correct format: rename node <old_name> <new_name>"); // PDF 14.3.7
        }
        return false;
    }

    bool tryParseListCommands(const string& in) {
        smatch matches;
        regex list_all_pattern(R"(^list$)");
        if (regex_match(in, matches, list_all_pattern)) {
            controller.handleListCommand(); return true;
        }
        regex list_comp_pattern(R"(^list ([RCLDVIEGHF])$)");
        if (regex_match(in, matches, list_comp_pattern)) {
            controller.handleListCommand(matches[1].str()); return true;
        }
        return false;
    }

    bool tryParsePrintCommands(const string& in) {
        smatch matches;
        regex print_pattern(R"(^print (TRAN|DC|AC) (.+)$)");
        if (regex_match(in, matches, print_pattern)) {
            try {
                controller.handlePrintCommand(matches[1].str(), matches[2].str());
            } catch (const logic_error& e) {
                string err_msg = e.what();
                // Assuming controller throws errors that contain these substrings from PDF 15.5
                if (err_msg.find("not found in circuit") != string::npos || err_msg.find("not found in circuit") != string::npos ) {
                    throw;
                }
                // If not a specific "not found" error, assume generic syntax error for print command
                throw logic_error("Syntax error in command"); // PDF 15.5.3
            }
            return true;
        }
        return false;
    }

    bool tryParseFileAndMenuCommands(const string& in) {
        smatch matches;
        regex new_file_pattern(R"(^NewFile\s+(.+)$)");
        if (regex_match(in, matches, new_file_pattern)) {
            controller.handleNewFileCommand(matches[1].str()); return true;
        }
        regex show_schematics_pattern(R"(^show existing schematics$)");
        if (regex_match(in, matches, show_schematics_pattern)) {
            controller.handleShowExistingSchematicsCommand(); return true;
        }

        if (controller.is_expecting_schematic_choice) {
            // No specific regex here, pass directly to controller to handle all cases
            // (number, "return", or invalid text like "draft1", "rreturn")
            controller.handleChooseSchematic(in);
            return true;
        }
        return false;
    }


public:
    explicit CommandParser(centralController& ctrl) : controller(ctrl) {}

    void processInput(const string& in) {
        if (in.empty()) {
            // cout << "Debug: Empty input ignored." << endl; // Optional debug
            return;
        }
        // cout << "Debug: Processing input: \"" << in << "\"" << endl; // Optional debug

        // Order of parsing: Context-dependent (menu) first.
        if (tryParseFileAndMenuCommands(in)) return;
        // Then general utility commands
        if (tryParseNodeCommands(in)) return;
        if (tryParseListCommands(in)) return;
        if (tryParsePrintCommands(in)) return;
        // Then element/source definitions (often start with "add" or "delete")
        if (tryParseElementCommands(in)) return; // Handles R,C,L,D,GND
        if (tryParseSourceCommands(in)) return; // Handles V,I,E,G,H,F

        // If no command matched:
        // PDF uses "Error: Syntax error" for malformed element def (e.g. 13.1.5.2)
        // PDF uses "Syntax error in command" for print (15.5.3).
        // For a completely unknown command, let's use a general message.
        throw logic_error("Error: Unknown command or invalid syntax. -> " + in);
    }
};

int main() {
    centralController controller_instance;
    CommandParser parser(controller_instance);

    vector<string> test_commands = {
        // Section 14
        "nodes",
        "list",
        "list R",
        "list V",
        "rename node N001 Vout",
        "rename node N002",
        // Section 15
        "print TRAN V(n001) I(R1)",
        "print DC Vsource 0 5 0.1 V(out)",
        // Section 17 (NewFile is here, show existing schematics is in menu_inputs)
        "NewFile /path/to/circuit.txt",
        // Section 13 Elements
        "add R1 N1 N2 1k",
        "add Rload N_in N_out 2.2Meg",
        "add C1 N2 0 10u",
        "add L1 N1 N_intermediate 1m", // Changed from 1mH to 1m to match PDF value format
        "add D1 N_intermediate N2 D",
        "add D_zener Vcc Vout Z",
        "delete R1",
        "add GND 0",
        "delete GND 0",
        "add RX N1 N2 0",
        "add R2 N3 N4 -100",
        "add R3 N5 N6 10p",
        "add R4 N7 N8 1.2e-3",
        "add R5 N9 N10 1.2e",
        // Section 18 Sources
        "add Vsrc N_plus N_minus 5",
        "add Isrc N_in N_out 100m",
        "add Vosc P1 P2 SIN(0, 5, 1k)",
        "add Vpulse P3 P4 SIN( 1, 2.5, 500 )",
        "add Eamp Nout Nin Nctrl1 Nctrl2 100",
        "add Gtrans N_curr_out N_curr_in N_volt_ctrl1 N_volt_ctrl2 0.1",
        "add Hsense N_volt_out N_volt_gnd Vcontrol_current_src 50",
        "add Fmirror N_curr_out_2 N_curr_gnd_2 Vcontrol_current_src_for_F 1",
        "delete Vsrc",
        // Errors
        "add Rtest N1 N2 value_without_prefix",
        "add Ctest N1 N2 100pF", // valuate should handle 'pF'. 'F' might be an issue if not stripped.
                                 // Current valuate expects 'p' and then checkDouble on "100".
        "add Ltest N1 N2 k",
        "add L_bad_unit N1 N2 1mH", // This should now cause "Invalid value format"
        "unknown command here",
        "add",
        "delete"
    };

    cout << "--- Starting Command Tests ---" << endl;
    for (const string& cmd : test_commands) {
        cout << "\nInput: \"" << cmd << "\"" << endl;
        try {
            parser.processInput(cmd);
        } catch (const logic_error& e) {
            cerr << "Exception: " << e.what() << endl;
        }
    }
    cout << "\n--- Testing Schematic Choice Menu ---" << endl;
    // These inputs will be passed to controller.handleChooseSchematic
    // if controller.is_expecting_schematic_choice is true
    string menu_inputs[] = {"show existing schematics", "5", "draft2", "1", "return"};
    for (const string& cmd : menu_inputs) {
        cout << "\nInput: \"" << cmd << "\"" << endl;
        try {
            parser.processInput(cmd);
        } catch (const logic_error& e) {
            cerr << "Exception: " << e.what() << endl;
        }
    }

    return 0;
}
