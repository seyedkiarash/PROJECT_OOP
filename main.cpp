// // // ///////////////////////////////////////////////
// // // // #include <iomanip>   // برای форматирование вывода (std::fixed, std::setprecision)
// // // // #include <iostream>
// // // // #include <map>       // برای نگاشت گره‌ها به اندیس ماتریس
// // // // #include <stdexcept> // برای کلاس‌های استثنا مانند std::invalid_argument
// // // // #include <string>
// // // // #include <vector>
// // // //
// // // // // کتابخانه Eigen برای عملیات ماتریسی
// // // // #include "Eigen/Dense"
// // // //
// // // // // استفاده از فضای نام std برای سادگی در این مثال
// // // // // در پروژه‌های بزرگتر، بهتر است از پیشوند std:: استفاده شود یا using declarations محدودتری به کار رود.
// // // // using namespace std;
// // // //
// // // // // تعریف کلاس گره (Node)
// // // // class Node {
// // // // private:
// // // //     double voltage; // ولتاژ گره، به double تغییر داده شد
// // // //     string name;    // نام گره
// // // //
// // // // public:
// // // //     // سازنده کلاس گره
// // // //     Node(const string &name, double voltage = 0.0) { // مقدار پیش‌فرض ولتاژ 0.0 در نظر گرفته شد
// // // //         this->name = name;
// // // //         this->voltage = voltage;
// // // //     }
// // // //
// // // //     // دریافت نام گره
// // // //     string getName() const {
// // // //         return name;
// // // //     }
// // // //
// // // //     // دریافت ولتاژ گره
// // // //     double getVoltage() const {
// // // //         return voltage;
// // // //     }
// // // //
// // // //     // تنظیم ولتاژ گره
// // // //     void setVoltage(double v) {
// // // //         voltage = v;
// // // //     }
// // // //
// // // //     // بررسی اینکه آیا گره، گره زمین (گراند) است یا خیر
// // // //     bool isGround() const {
// // // //         return name == "0" || name == "GND" || name == "gnd"; // نام‌های رایج برای زمین
// // // //     }
// // // // };
// // // //
// // // // // تعریف کلاس پایه برای المان‌های مداری (Element)
// // // // class Element {
// // // // protected:
// // // //     Node *node1, *node2; // اشاره‌گر به گره‌های متصل به المان
// // // //     string name;         // نام المان
// // // //
// // // // public:
// // // //     // سازنده کلاس المان
// // // //     Element(Node* n1, Node* n2, const string &name) {
// // // //         if (!n1 || !n2) {
// // // //             throw std::invalid_argument("گره‌های المان نمی‌توانند تهی (null) باشند.");
// // // //         }
// // // //         this->node1 = n1;
// // // //         this->node2 = n2;
// // // //         this->name = name;
// // // //     }
// // // //
// // // //     // مخرب مجازی پیش‌فرض
// // // //     virtual ~Element() = default;
// // // //
// // // //     // دریافت نام المان
// // // //     string getName() const { return name; }
// // // //
// // // //     // دریافت گره اول
// // // //     Node* getNode1() const { return node1; }
// // // //
// // // //     // دریافت گره دوم
// // // //     Node* getNode2() const { return node2; }
// // // //
// // // //     // تابع مجازی خالص برای دریافت نوع المان (باید در کلاس‌های مشتق پیاده‌سازی شود)
// // // //     virtual string getType() const = 0;
// // // //
// // // //     // تابع مجازی خالص برای دریافت مقدار مشخصه المان (مقاومت، ولتاژ منبع، جریان منبع و ...)
// // // //     virtual double getValue() const = 0;
// // // //
// // // //     // تابع مجازی برای دریافت جریان عبوری از المان (می‌تواند در کلاس‌های مشتق بازنویسی شود)
// // // //     // برای المان‌هایی مانند مقاومت پس از حل مدار محاسبه می‌شود
// // // //     // برای منابع ولتاژ، این یکی از مجهولات سیستم MNA است
// // // //     virtual double getCurrent() const {
// // // //         return 0.0; // پیاده‌سازی پیش‌فرض
// // // //     }
// // // //
// // // //     // تابع مجازی برای تنظیم جریان عبوری از المان (مربوط به منابع ولتاژ)
// // // //     virtual void setCurrent(double current) {
// // // //         // پیاده‌سازی پیش‌فرض، برای جلوگیری از هشدار پارامتر استفاده‌نشده
// // // //         (void)current;
// // // //     }
// // // // };
// // // //
// // // // // کلاس مقاومت (Resistor) مشتق‌شده از Element
// // // // class Resistor : public Element {
// // // // private:
// // // //     double resistance; // مقدار مقاومت
// // // //
// // // // public:
// // // //     Resistor(Node* n1, Node* n2, const string &name, double res) : Element(n1, n2, name) {
// // // //         if (res <= 0) {
// // // //             // مطابق فایل PDF، بخش ۱۹ (مدیریت خطاها)
// // // //             throw std::invalid_argument("خطا: مقدار مقاومت باید مثبت باشد. المان: " + name);
// // // //         }
// // // //         this->resistance = res;
// // // //     }
// // // //
// // // //     string getType() const override { return "Resistor"; }
// // // //     double getValue() const override { return resistance; } // مقدار مقاومت را برمی‌گرداند
// // // //
// // // //     // جریان عبوری از مقاومت طبق قانون اهم (پس از مشخص شدن ولتاژ گره‌ها)
// // // //     double getCurrent() const override {
// // // //         if (node1 && node2) {
// // // //             return (node1->getVoltage() - node2->getVoltage()) / resistance;
// // // //         }
// // // //         return 0.0;
// // // //     }
// // // // };
// // // //
// // // // // کلاس منبع ولتاژ مستقل (VoltageSource) مشتق‌شده از Element
// // // // class VoltageSource : public Element {
// // // // private:
// // // //     double voltageValue;         // مقدار ولتاژ منبع
// // // //     double currentThroughSource; // جریان عبوری از منبع (یکی از مجهولات MNA)
// // // //
// // // // public:
// // // //     VoltageSource(Node* n1, Node* n2, const string &name, double val) : Element(n1, n2, name) {
// // // //         this->voltageValue = val;
// // // //         this->currentThroughSource = 0.0; // مقدار اولیه
// // // //     }
// // // //
// // // //     string getType() const override { return "VoltageSource"; }
// // // //     double getValue() const override { return voltageValue; } // مقدار ولتاژ تنظیم‌شده منبع را برمی‌گرداند
// // // //
// // // //     double getCurrent() const override {
// // // //         return currentThroughSource;
// // // //     }
// // // //     void setCurrent(double current) override {
// // // //         this->currentThroughSource = current;
// // // //     }
// // // // };
// // // //
// // // // // کلاس منبع جریان مستقل (CurrentSource) مشتق‌شده از Element
// // // // class CurrentSource : public Element {
// // // // private:
// // // //     double currentValue; // مقدار جریان منبع
// // // //
// // // // public:
// // // //     // گره n1 گرهی است که جریان از آن خارج می‌شود، گره n2 گرهی است که جریان به آن وارد می‌شود
// // // //     CurrentSource(Node* n1, Node* n2, const string &name, double val) : Element(n1, n2, name) {
// // // //         this->currentValue = val;
// // // //     }
// // // //
// // // //     string getType() const override { return "CurrentSource"; }
// // // //     double getValue() const override { return currentValue; } // مقدار جریان تنظیم‌شده منبع را برمی‌گرداند
// // // //
// // // //     // ولتاژ دو سر منبع جریان توسط مدار تعیین می‌شود و جزو مجهولات مستقیم برای این المان نیست
// // // // };
// // // //
// // // //
// // // // // کلاس مسئول تشکیل ماتریس‌های MNA
// // // // class MakingMNA {
// // // // private:
// // // //     vector<Node*> allNodesInCircuit;     // لیست تمام گره‌های موجود در مدار
// // // //     vector<Element*> elementsInCircuit;  // لیست تمام المان‌های موجود در مدار
// // // //     Node* groundNodeRef;                 // اشاره‌گر به گره زمین
// // // //
// // // //     // نگاشت‌ها برای تشکیل ماتریس
// // // //     map<Node*, int> nodeToIndexMap;          // نگاشت گره‌های غیرزمین به اندیس سطر/ستون در ماتریس MNA
// // // //     vector<Node*> orderedNonGroundNodes;     // لیست مرتب‌شده گره‌های غیرزمین
// // // //     map<VoltageSource*, int> vsToIndexMap;   // نگاشت منابع ولتاژ به اندیس مجهول جریانشان در MNA
// // // //     vector<VoltageSource*> orderedVoltageSources; // لیست مرتب‌شده منابع ولتاژ
// // // //
// // // //     // متد داخلی برای ساخت نگاشت‌ها
// // // //     void buildNodeAndVoltageSourceMaps() {
// // // //         nodeToIndexMap.clear();
// // // //         orderedNonGroundNodes.clear();
// // // //         vsToIndexMap.clear();
// // // //         orderedVoltageSources.clear();
// // // //
// // // //         // تعیین گره زمین در صورت عدم تنظیم صریح
// // // //         if (!groundNodeRef) {
// // // //             for (Node* n : allNodesInCircuit) {
// // // //                 if (n->isGround()) {
// // // //                     groundNodeRef = n;
// // // //                     break;
// // // //                 }
// // // //             }
// // // //             if (!groundNodeRef) {
// // // //                 // مطابق فایل PDF، بخش ۱۹ (مدیریت خطاها)
// // // //                 throw std::runtime_error("خطا: گره زمین (گراند) در مدار تشخیص داده نشد. تحلیل امکان‌پذیر نیست.");
// // // //             }
// // // //         }
// // // //
// // // //         // شماره‌گذاری گره‌های غیرزمین
// // // //         int nodeIdx = 0;
// // // //         for (Node* node : allNodesInCircuit) {
// // // //             if (node != groundNodeRef) {
// // // //                 orderedNonGroundNodes.push_back(node);
// // // //                 nodeToIndexMap[node] = nodeIdx++;
// // // //             }
// // // //         }
// // // //
// // // //         // شماره‌گذاری منابع ولتاژ (برای مجهولات جریان آنها)
// // // //         int vsIdx = 0;
// // // //         for (Element* elem : elementsInCircuit) {
// // // //             if (auto vs = dynamic_cast<VoltageSource*>(elem)) {
// // // //                 orderedVoltageSources.push_back(vs);
// // // //                 vsToIndexMap[vs] = vsIdx++;
// // // //             }
// // // //         }
// // // //     }
// // // //
// // // // public:
// // // //     // سازنده
// // // //     MakingMNA() : groundNodeRef(nullptr) {}
// // // //
// // // //     // مخرب (در صورت نیاز به مدیریت حافظه پیچیده‌تر، باید تکمیل شود)
// // // //     ~MakingMNA() {
// // // //         // اگر گره‌ها و المان‌ها در این کلاس new شده باشند، باید delete شوند.
// // // //         // در این پیاده‌سازی، فرض بر این است که مدیریت حافظه آنها خارج از این کلاس انجام می‌شود.
// // // //     }
// // // //
// // // //     // افزودن گره به مدار
// // // //     void addNode(Node* node) {
// // // //         if (!node) return;
// // // //         allNodesInCircuit.push_back(node);
// // // //         if (node->isGround()) {
// // // //             if (groundNodeRef != nullptr && groundNodeRef != node) {
// // // //                 cout << "هشدار: چندین گره زمین تعریف شده است. از اولین گره زمین شناسایی‌شده استفاده می‌شود: "
// // // //                      << groundNodeRef->getName() << endl;
// // // //             } else if (groundNodeRef == nullptr) {
// // // //                 groundNodeRef = node;
// // // //             }
// // // //         }
// // // //     }
// // // //
// // // //     // افزودن المان به مدار
// // // //     void addElement(Element* element) {
// // // //         if (!element) return;
// // // //         elementsInCircuit.push_back(element);
// // // //     }
// // // //
// // // //     // تنظیم صریح گره زمین
// // // //     void setGroundNode(Node* gnd) {
// // // //         if (!gnd) {
// // // //             throw std::invalid_argument("گره زمین نمی‌تواند تهی (null) باشد.");
// // // //         }
// // // //         bool found = false;
// // // //         for(Node* n : allNodesInCircuit) {
// // // //             if(n == gnd) {
// // // //                 found = true;
// // // //                 break;
// // // //             }
// // // //         }
// // // //         if(!found) addNode(gnd); // اگر گره زمین هنوز به لیست گره‌ها اضافه نشده، اضافه کن
// // // //
// // // //         groundNodeRef = gnd;
// // // //     }
// // // //
// // // //     // تشکیل و دریافت ماتریس سیستم A در معادله Ax = Z
// // // //     Eigen::MatrixXd getSystemMatrixA() {
// // // //         buildNodeAndVoltageSourceMaps(); // اطمینان از به‌روز بودن نگاشت‌ها
// // // //
// // // //         int numNonGroundNodes = orderedNonGroundNodes.size();
// // // //         int numVoltageSources = orderedVoltageSources.size();
// // // //         int systemSize = numNonGroundNodes + numVoltageSources;
// // // //
// // // //         if (systemSize == 0) {
// // // //             // اگر مداری خالی یا فقط با یک گره زمین باشد
// // // //              throw std::runtime_error("خطا: مدار برای تحلیل بسیار کوچک است (فاقد گره‌های غیر زمین یا منابع ولتاژ).");
// // // //         }
// // // //
// // // //         Eigen::MatrixXd A = Eigen::MatrixXd::Zero(systemSize, systemSize);
// // // //
// // // //         // بخش G (مربوط به هدایت‌ها - مقاومت‌ها)
// // // //         for (Element* elem : elementsInCircuit) {
// // // //             if (auto res = dynamic_cast<Resistor*>(elem)) {
// // // //                 double conductance = 1.0 / res->getValue();
// // // //                 Node* n1 = res->getNode1();
// // // //                 Node* n2 = res->getNode2();
// // // //
// // // //                 // تاثیر بر قطر اصلی
// // // //                 if (n1 != groundNodeRef) {
// // // //                     A(nodeToIndexMap[n1], nodeToIndexMap[n1]) += conductance;
// // // //                 }
// // // //                 if (n2 != groundNodeRef) {
// // // //                     A(nodeToIndexMap[n2], nodeToIndexMap[n2]) += conductance;
// // // //                 }
// // // //                 // تاثیر بر عناصر خارج از قطر
// // // //                 if (n1 != groundNodeRef && n2 != groundNodeRef) {
// // // //                     A(nodeToIndexMap[n1], nodeToIndexMap[n2]) -= conductance;
// // // //                     A(nodeToIndexMap[n2], nodeToIndexMap[n1]) -= conductance;
// // // //                 }
// // // //             }
// // // //         }
// // // //
// // // //         // بخش B و C (مربوط به منابع ولتاژ)
// // // //         // طبق توضیحات فایل PDF بخش ۵.۲
// // // //         for (size_t i = 0; i < orderedVoltageSources.size(); ++i) {
// // // //             VoltageSource* vs = orderedVoltageSources[i];
// // // //             Node* n_plus = vs->getNode1();  // گره مثبت منبع ولتاژ
// // // //             Node* n_minus = vs->getNode2(); // گره منفی منبع ولتاژ
// // // //             int vsMNAIndex = vsToIndexMap[vs]; // اندیس مجهول جریان این منبع ولتاژ
// // // //
// // // //             if (n_plus != groundNodeRef) {
// // // //                 int nodeIdx = nodeToIndexMap[n_plus];
// // // //                 A(nodeIdx, numNonGroundNodes + vsMNAIndex) += 1.0;  // بخش B
// // // //                 A(numNonGroundNodes + vsMNAIndex, nodeIdx) += 1.0;  // بخش C
// // // //             }
// // // //             if (n_minus != groundNodeRef) {
// // // //                 int nodeIdx = nodeToIndexMap[n_minus];
// // // //                 A(nodeIdx, numNonGroundNodes + vsMNAIndex) -= 1.0; // بخش B
// // // //                 A(numNonGroundNodes + vsMNAIndex, nodeIdx) -= 1.0; // بخش C
// // // //             }
// // // //         }
// // // //
// // // //         // بخش D ماتریس A برای منابع ولتاژ مستقل ایده‌آل صفر است.
// // // //         // Eigen::MatrixXd::Zero قبلاً این بخش را صفر کرده است.
// // // //
// // // //         return A;
// // // //     }
// // // //
// // // //     // تشکیل و دریافت بردار سمت راست Z (یا B در برخی منابع) در معادله Ax = Z
// // // //     Eigen::VectorXd getSystemVectorZ() {
// // // //         // فرض می‌شود buildNodeAndVoltageSourceMaps() قبلاً فراخوانی شده است
// // // //         int numNonGroundNodes = orderedNonGroundNodes.size();
// // // //         int numVoltageSources = orderedVoltageSources.size();
// // // //         int systemSize = numNonGroundNodes + numVoltageSources;
// // // //
// // // //         if (systemSize == 0 && numNonGroundNodes == 0) { // بررسی مجدد برای حالت خاص بردار Z
// // // //              // اگرچه getSystemMatrixA این را بررسی می‌کند، برای اطمینان
// // // //              Eigen::VectorXd Z_empty(0);
// // // //              return Z_empty;
// // // //         }
// // // //
// // // //
// // // //         Eigen::VectorXd Z = Eigen::VectorXd::Zero(systemSize);
// // // //
// // // //         // بخش J (مربوط به منابع جریان)
// // // //         // طبق توضیحات فایل PDF بخش ۵.۲
// // // //         for (Element* elem : elementsInCircuit) {
// // // //             if (auto cs = dynamic_cast<CurrentSource*>(elem)) {
// // // //                 Node* n_from = cs->getNode1(); // جریان از این گره خارج می‌شود
// // // //                 Node* n_to = cs->getNode2();   // جریان به این گره وارد می‌شود
// // // //                 double currentValue = cs->getValue();
// // // //
// // // //                 if (n_to != groundNodeRef) { // جریان ورودی به گره n_to
// // // //                     Z(nodeToIndexMap[n_to]) += currentValue;
// // // //                 }
// // // //                 if (n_from != groundNodeRef) { // جریان خروجی از گره n_from
// // // //                     Z(nodeToIndexMap[n_from]) -= currentValue;
// // // //                 }
// // // //             }
// // // //         }
// // // //
// // // //         // بخش E (مربوط به مقادیر منابع ولتاژ)
// // // //         // طبق توضیحات فایل PDF بخش ۵.۲
// // // //         for (size_t i = 0; i < orderedVoltageSources.size(); ++i) {
// // // //             VoltageSource* vs = orderedVoltageSources[i];
// // // //             int vsMNAIndex = vsToIndexMap[vs];
// // // //             Z(numNonGroundNodes + vsMNAIndex) = vs->getValue();
// // // //         }
// // // //         return Z;
// // // //     }
// // // //
// // // //     // متدهای Getter برای دسترسی به لیست‌های داخلی (برای به‌روزرسانی وضعیت پس از حل)
// // // //     const vector<Node*>& getOrderedNonGroundNodes() const {
// // // //         return orderedNonGroundNodes;
// // // //     }
// // // //
// // // //     const vector<VoltageSource*>& getOrderedVoltageSources() const {
// // // //         return orderedVoltageSources;
// // // //     }
// // // //     // برای دسترسی به نگاشت vs->index برای بروزرسانی جریان vs ها
// // // //      const map<VoltageSource*, int>& getVoltageSourceToIndexMap() const {
// // // //         return vsToIndexMap;
// // // //     }
// // // //      const vector<Element*>& getAllElements() const { // برای محاسبه جریان مقاومت‌ها پس از حل
// // // //         return elementsInCircuit;
// // // //     }
// // // // };
// // // //
// // // // // کلاس حل‌کننده سیستم معادلات MNA
// // // // class MNASolver {
// // // // public:
// // // //     MNASolver() {}
// // // //
// // // //     // حل سیستم AX = Z و برگرداندن بردار X (شامل ولتاژ گره‌ها و سپس جریان منابع ولتاژ)
// // // //     Eigen::VectorXd solve(const Eigen::MatrixXd& A, const Eigen::VectorXd& Z) {
// // // //         if (A.rows() != A.cols() || A.rows() != Z.size()) {
// // // //             throw std::runtime_error("خطا: ابعاد ماتریس و بردار برای حل سازگار نیستند.");
// // // //         }
// // // //         if (A.rows() == 0) {
// // // //              throw std::runtime_error("خطا: سیستم معادلات برای حل خالی است.");
// // // //         }
// // // //
// // // //         // استفاده از تجزیه LU با Partial Pivoting برای پایداری عددی
// // // //         Eigen::PartialPivLU<Eigen::MatrixXd> lu(A);
// // // //         if (!lu.isInvertible()) {
// // // //             // این خطا می‌تواند برای مدارهای تعریف‌نشده (مثلاً بخش‌های شناور) یا ماتریس‌های تکین رخ دهد.
// // // //             // مطابق فایل PDF، بخش ۱۹ (مدیریت خطاها - مدار ناپیوسته)
// // // //             throw std::runtime_error("خطا: ماتریس سیستم تکین یا بدحالت است. مدار ممکن است قابل حل نباشد (مثلاً بخش‌های شناور، منابع ولتاژ اضافی و ...).");
// // // //         }
// // // //         return lu.solve(Z);
// // // //     }
// // // //
// // // //     // به‌روزرسانی ولتاژ گره‌ها و جریان منابع ولتاژ پس از حل سیستم
// // // //     void updateCircuitState(const Eigen::VectorXd& X, MakingMNA& mnaCircuit) {
// // // //         const auto& nonGroundNodes = mnaCircuit.getOrderedNonGroundNodes();
// // // //         const auto& voltageSources = mnaCircuit.getOrderedVoltageSources(); // این لیست مرتب است
// // // //         const auto& vsMap = mnaCircuit.getVoltageSourceToIndexMap(); // برای گرفتن اندیس صحیح هر vs
// // // //
// // // //         int numNonGroundNodes = nonGroundNodes.size();
// // // //
// // // //         // به‌روزرسانی ولتاژ گره‌ها
// // // //         for (int i = 0; i < numNonGroundNodes; ++i) {
// // // //             nonGroundNodes[i]->setVoltage(X(i));
// // // //         }
// // // //
// // // //         // به‌روزرسانی جریان عبوری از منابع ولتاژ
// // // //         // ترتیب در orderedVoltageSources با ترتیب اندیس‌ها در vsMap مطابقت دارد
// // // //         for (size_t i = 0; i < voltageSources.size(); ++i) {
// // // //             VoltageSource* vs = voltageSources[i]; // vs فعلی از لیست مرتب شده
// // // //             int vsMNAIndex = vsMap.at(vs); // اندیس این vs در بخش جریان‌های مجهول ماتریس MNA
// // // //             vs->setCurrent(X(numNonGroundNodes + vsMNAIndex));
// // // //         }
// // // //     }
// // // // };
// // // //
// // // //
// // // // // تابع اصلی برای تست
// // // // int main() {
// // // //     // تنظیم دقت خروجی برای اعداد اعشاری
// // // //     cout << fixed << setprecision(6);
// // // //
// // // //     // ۱. ایجاد گره‌ها
// // // //     Node n1("1"), n2("2"), n_gnd("0"); // گره "0" یا "GND" معمولاً زمین است
// // // //
// // // //     // ۲. ایجاد مدیر مدار و افزودن گره‌ها
// // // //     MakingMNA circuit;
// // // //     circuit.addNode(&n1);
// // // //     circuit.addNode(&n2);
// // // //     circuit.addNode(&n_gnd);
// // // //     // circuit.setGroundNode(&n_gnd); // تنظیم صریح گره زمین (اگرچه به طور خودکار نیز شناسایی می‌شود)
// // // //
// // // //     // ۳. ایجاد المان‌ها و افزودن آنها به مدار
// // // //     try {
// // // //         // مثال از فایل PDF بخش ۹ (مدار RC): V1=5V, R=1k, C=1uF (اینجا C را با مقاومت دوم جایگزین می‌کنیم برای مثال DC)
// // // //         // V1 بین گره 1 و زمین، R1 بین 1 و 2، R2 بین 2 و زمین
// // // //         VoltageSource vs(&n1, &n_gnd, "V1", 5.0);   // منبع ولتاژ 5 ولت بین گره 1 و زمین
// // // //         Resistor r1(&n1, &n2, "R1", 1000.0);     // مقاومت 1 کیلواهم بین گره 1 و 2
// // // //         Resistor r2(&n2, &n_gnd, "R2", 2000.0);   // مقاومت 2 کیلواهم بین گره 2 و زمین
// // // //         // CurrentSource cs(&n_gnd, &n2, "I1", 0.001); // مثال: منبع جریان 1 میلی‌آمپر از زمین به گره 2
// // // //
// // // //         circuit.addElement(&vs);
// // // //         circuit.addElement(&r1);
// // // //         circuit.addElement(&r2);
// // // //         // circuit.addElement(&cs);
// // // //
// // // //         // ۴. دریافت ماتریس‌های MNA
// // // //         Eigen::MatrixXd A = circuit.getSystemMatrixA();
// // // //         Eigen::VectorXd Z = circuit.getSystemVectorZ();
// // // //
// // // //         cout << "ماتریس سیستم A:\n" << A << endl << endl;
// // // //         cout << "بردار سیستم Z:\n" << Z << endl << endl;
// // // //
// // // //         // ۵. حل سیستم
// // // //         MNASolver solver;
// // // //         Eigen::VectorXd X = solver.solve(A, Z);
// // // //
// // // //         cout << "بردار حل X (شامل ولتاژ گره‌ها و سپس جریان منابع ولتاژ):\n" << X << endl << endl;
// // // //
// // // //         // ۶. به‌روزرسانی ولتاژ گره‌ها و جریان منابع ولتاژ در اشیاء مربوطه
// // // //         solver.updateCircuitState(X, circuit);
// // // //
// // // //         // ۷. چاپ نتایج
// // // //         cout << "ولتاژ گره‌ها پس از حل:" << endl;
// // // //         for (const auto* node : circuit.getOrderedNonGroundNodes()) {
// // // //             cout << "گره " << node->getName() << ": " << node->getVoltage() << " V" << endl;
// // // //         }
// // // //
// // // //         cout << "\nجریان عبوری از منابع ولتاژ:" << endl;
// // // //         for (const auto* vs_elem : circuit.getOrderedVoltageSources()) {
// // // //             cout << "جریان عبوری از " << vs_elem->getName() << ": " << vs_elem->getCurrent() << " A" << endl;
// // // //         }
// // // //
// // // //         cout << "\nجریان عبوری از مقاومت‌ها (محاسبه‌شده پس از حل):" << endl;
// // // //         for (const auto* elem : circuit.getAllElements()) {
// // // //            if (const Resistor* res = dynamic_cast<const Resistor*>(elem)) {
// // // //                cout << "جریان عبوری از " << res->getName() << " (" << res->getNode1()->getName() << "->" << res->getNode2()->getName() << "): "
// // // //                     << res->getCurrent() << " A" << endl;
// // // //            }
// // // //         }
// // // //
// // // //
// // // //     } catch (const std::exception& e) {
// // // //         cerr << "یک خطا رخ داد: " << e.what() << endl;
// // // //         // مثال‌هایی از پیام‌های خطا در فایل PDF:
// // // //         // "خطا: مقدار مقاومت نمی‌تواند صفر یا منفی باشد."
// // // //         // "خطا: گره زمین (گراند) در مدار تشخیص داده نشد."
// // // //         // "خطا: مقدار منفی یا صفر برای یک المان نامعتبر است."
// // // //         // "خطا: ماتریس سیستم تکین یا بدحالت است..."
// // // //     }
// // // //
// // // //     return 0;
// // // // }
// // //
// // // ////////////////////////////////////////////////
// // // #include <iostream>
// // // #include <vector>
// // // #include <string>
// // // #include <stdexcept> // برای کلاس‌های استثنا مانند std::invalid_argument
// // // #include <map>       // برای نگاشت گره‌ها به اندیس ماتریس
// // // #include <iomanip>   // برای форматирование вывода (std::fixed, std::setprecision)
// // // #include <cmath>     // برای std::abs و std::fabs
// // // #include <algorithm> // برای std::swap
// // // #include <regex>
// // //
// // // // استفاده از فضای نام std برای سادگی در این مثال
// // // // در پروژه‌های بزرگتر، بهتر است از پیشوند std:: استفاده شود یا using declarations محدودتری به کار رود.
// // // using namespace std;
// // //
// // // // تعریف نوع برای ماتریس و بردار
// // // using Matrix = vector<vector<double>>;
// // // using Vector = vector<double>;
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
// // //             throw std::invalid_argument("Nodes cannot be null.");
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
// // //             throw std::invalid_argument("Error:value of Resistor cannot be negative. Resistor Name: " + name);
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
// // //                 throw std::runtime_error("Error: invable Ground Node. impossiple solving!");
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
// // //                 cout << "Warning: there is some Ground Node. first Node has been used: "
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
// // //             throw std::invalid_argument("Ground Node cannot be null.");
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
// // //     Matrix getSystemMatrixA() {
// // //         buildNodeAndVoltageSourceMaps(); // اطمینان از به‌روز بودن نگاشت‌ها
// // //
// // //         int numNonGroundNodes = orderedNonGroundNodes.size();
// // //         int numVoltageSources = orderedVoltageSources.size();
// // //         int systemSize = numNonGroundNodes + numVoltageSources;
// // //
// // //         if (systemSize == 0) {
// // //             // اگر مداری خالی یا فقط با یک گره زمین باشد
// // //              throw std::runtime_error("Error: there is no eny Node.");
// // //         }
// // //
// // //         Matrix A(systemSize, Vector(systemSize, 0.0));
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
// // //                     A[nodeToIndexMap[n1]][nodeToIndexMap[n1]] += conductance;
// // //                 }
// // //                 if (n2 != groundNodeRef) {
// // //                     A[nodeToIndexMap[n2]][nodeToIndexMap[n2]] += conductance;
// // //                 }
// // //                 // تاثیر بر عناصر خارج از قطر
// // //                 if (n1 != groundNodeRef && n2 != groundNodeRef) {
// // //                     A[nodeToIndexMap[n1]][nodeToIndexMap[n2]] -= conductance;
// // //                     A[nodeToIndexMap[n2]][nodeToIndexMap[n1]] -= conductance;
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
// // //                 A[nodeIdx][numNonGroundNodes + vsMNAIndex] += 1.0;  // بخش B
// // //                 A[numNonGroundNodes + vsMNAIndex][nodeIdx] += 1.0;  // بخش C
// // //             }
// // //             if (n_minus != groundNodeRef) {
// // //                 int nodeIdx = nodeToIndexMap[n_minus];
// // //                 A[nodeIdx][numNonGroundNodes + vsMNAIndex] -= 1.0; // بخش B
// // //                 A[numNonGroundNodes + vsMNAIndex][nodeIdx] -= 1.0; // بخش C
// // //             }
// // //         }
// // //
// // //         // بخش D ماتریس A برای منابع ولتاژ مستقل ایده‌آل صفر است.
// // //         // ماتریس با صفرها مقداردهی اولیه شده است.
// // //
// // //         return A;
// // //     }
// // //
// // //     // تشکیل و دریافت بردار سمت راست Z (یا B در برخی منابع) در معادله Ax = Z
// // //     Vector getSystemVectorZ() {
// // //         // فرض می‌شود buildNodeAndVoltageSourceMaps() قبلاً فراخوانی شده است
// // //         int numNonGroundNodes = orderedNonGroundNodes.size();
// // //         int numVoltageSources = orderedVoltageSources.size();
// // //         int systemSize = numNonGroundNodes + numVoltageSources;
// // //
// // //         if (systemSize == 0 && numNonGroundNodes == 0) {
// // //              Vector Z_empty; // بردار خالی
// // //              return Z_empty;
// // //         }
// // //
// // //         Vector Z(systemSize, 0.0);
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
// // //                     Z[nodeToIndexMap[n_to]] += currentValue;
// // //                 }
// // //                 if (n_from != groundNodeRef) { // جریان خروجی از گره n_from
// // //                     Z[nodeToIndexMap[n_from]] -= currentValue;
// // //                 }
// // //             }
// // //         }
// // //
// // //         // بخش E (مربوط به مقادیر منابع ولتاژ)
// // //         // طبق توضیحات فایل PDF بخش ۵.۲
// // //         for (size_t i = 0; i < orderedVoltageSources.size(); ++i) {
// // //             VoltageSource* vs = orderedVoltageSources[i];
// // //             int vsMNAIndex = vsToIndexMap[vs]; // این اندیس برای vs در orderedVoltageSources است
// // //             Z[numNonGroundNodes + vsMNAIndex] = vs->getValue();
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
// // //     // پیاده‌سازی با حذف گاوسی و محورگیری جزئی
// // //     Vector solve(Matrix A, Vector Z) { // کپی از A و Z چون در حین عملیات تغییر می‌کنند
// // //         int n = A.size();
// // //         if (n == 0 || A[0].size() != static_cast<size_t>(n) || Z.size() != static_cast<size_t>(n)) {
// // //             throw std::runtime_error("Error: solving problem.");
// // //         }
// // //
// // //         const double EPS = 1e-9; // یک مقدار کوچک برای بررسی تکین بودن
// // //
// // //         for (int i = 0; i < n; ++i) {
// // //             // محورگیری جزئی: یافتن بزرگترین عنصر در ستون فعلی (از سطر i به پایین)
// // //             int pivot_row = i;
// // //             for (int k = i + 1; k < n; ++k) {
// // //                 if (std::abs(A[k][i]) > std::abs(A[pivot_row][i])) {
// // //                     pivot_row = k;
// // //                 }
// // //             }
// // //
// // //             // جابجایی سطرها در A و Z
// // //             if (pivot_row != i) {
// // //                 std::swap(A[i], A[pivot_row]);
// // //                 std::swap(Z[i], Z[pivot_row]);
// // //             }
// // //
// // //             // بررسی تکین بودن (اگر عنصر روی قطر صفر یا خیلی کوچک باشد)
// // //             if (std::abs(A[i][i]) < EPS) {
// // //                 throw std::runtime_error("Error: solving problem for system.");
// // //             }
// // //
// // //             // نرمال‌سازی سطر i (عنصر قطری را 1 کن)
// // //             // این مرحله اختیاری است اما می‌تواند به پایداری کمک کند، گرچه در اینجا مستقیماً برای حذف استفاده می‌کنیم.
// // //             // برای سادگی، مستقیماً از A[i][i] به عنوان مقسوم‌علیه استفاده می‌کنیم.
// // //
// // //             // حذف عناصر زیر قطر در ستون i
// // //             for (int k = i + 1; k < n; ++k) {
// // //                 double factor = A[k][i] / A[i][i];
// // //                 for (int j = i; j < n; ++j) { // از ستون i شروع کن
// // //                     A[k][j] -= factor * A[i][j];
// // //                 }
// // //                 Z[k] -= factor * Z[i];
// // //             }
// // //         }
// // //
// // //         // جایگزینی پسرو (Back Substitution)
// // //         Vector X(n);
// // //         for (int i = n - 1; i >= 0; --i) {
// // //             double sum = 0.0;
// // //             for (int j = i + 1; j < n; ++j) {
// // //                 sum += A[i][j] * X[j];
// // //             }
// // //             if (std::abs(A[i][i]) < EPS) { // بررسی مجدد برای تقسیم بر صفر در جایگزینی پسرو
// // //                  throw std::runtime_error("Error: solving problem for system.");
// // //             }
// // //             X[i] = (Z[i] - sum) / A[i][i];
// // //         }
// // //         return X;
// // //     }
// // //
// // //
// // //     // به‌روزرسانی ولتاژ گره‌ها و جریان منابع ولتاژ پس از حل سیستم
// // //     void updateCircuitState(const Vector& X, MakingMNA& mnaCircuit) {
// // //         const auto& nonGroundNodes = mnaCircuit.getOrderedNonGroundNodes();
// // //         const auto& voltageSources = mnaCircuit.getOrderedVoltageSources();
// // //         const auto& vsMap = mnaCircuit.getVoltageSourceToIndexMap();
// // //
// // //         int numNonGroundNodes = nonGroundNodes.size();
// // //
// // //         if (X.size() != numNonGroundNodes + voltageSources.size()) {
// // //             throw std::runtime_error("Error: solving problem.");
// // //         }
// // //
// // //         // به‌روزرسانی ولتاژ گره‌ها
// // //         for (size_t i = 0; i < static_cast<size_t>(numNonGroundNodes); ++i) {
// // //             nonGroundNodes[i]->setVoltage(X[i]);
// // //         }
// // //
// // //         // به‌روزرسانی جریان عبوری از منابع ولتاژ
// // //         for (size_t i = 0; i < voltageSources.size(); ++i) {
// // //             VoltageSource* vs = voltageSources[i];
// // //             // اندیس vs در بخش جریان‌های مجهول ماتریس MNA
// // //             // vsMap کلیدش VoltageSource* و مقدارش اندیس اولیه است (0, 1, 2, ...)
// // //             // این اندیس‌ها باید با ترتیب در orderedVoltageSources مطابقت داشته باشند.
// // //             // بنابراین، اندیس در بردار X برابر است با numNonGroundNodes + (اندیس vs در orderedVoltageSources)
// // //             // یا اگر vsMap اندیس صحیح را برای vs در X بدهد (یعنی vsMap[vs] اندیس نهایی در X باشد)
// // //             // در پیاده‌سازی فعلی MakingMNA، vsMap اندیس‌های 0, 1, ... را برای vs ها برمی‌گرداند.
// // //             int vsMNAIndex = vsMap.at(vs); // این اندیس 0, 1, ... برای منابع ولتاژ است
// // //             vs->setCurrent(X[numNonGroundNodes + vsMNAIndex]);
// // //         }
// // //     }
// // // };
// // //
// // //
// // // // تابع کمکی برای چاپ ماتریس
// // // void printMatrix(const Matrix& mat, const string& name) {
// // //     cout << name << ":\n";
// // //     if (mat.empty()) {
// // //         cout << "(empty)\n";
// // //         return;
// // //     }
// // //     for (const auto& row : mat) {
// // //         for (double val : row) {
// // //             cout << setw(10) << val << " ";
// // //         }
// // //         cout << endl;
// // //     }
// // //     cout << endl;
// // // }
// // //
// // // // تابع کمکی برای چاپ بردار
// // // void printVector(const Vector& vec, const string& name) {
// // //     cout << name << ":\n";
// // //     if (vec.empty()) {
// // //         cout << "(empty)\n";
// // //         return;
// // //     }
// // //     for (double val : vec) {
// // //         cout << setw(10) << val << " ";
// // //     }
// // //     cout << endl << endl;
// // // }
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
// // //         Matrix A = circuit.getSystemMatrixA();
// // //         Vector Z = circuit.getSystemVectorZ();
// // //
// // //         printMatrix(A, "A Matrix");
// // //         printVector(Z, "Z Matrix");
// // //
// // //
// // //         // ۵. حل سیستم
// // //         MNASolver solver;
// // //         Vector X = solver.solve(A, Z); // A و Z به صورت کپی ارسال می‌شوند
// // //
// // //         printVector(X, "X vector");
// // //
// // //
// // //         // ۶. به‌روزرسانی ولتاژ گره‌ها و جریان منابع ولتاژ در اشیاء مربوطه
// // //         solver.updateCircuitState(X, circuit);
// // //
// // //         // ۷. چاپ نتایج
// // //         cout << "Voltage of Nodes after solving: " << endl;
// // //         for (const auto* node : circuit.getOrderedNonGroundNodes()) {
// // //             cout << "Node " << node->getName() << ": " << node->getVoltage() << " V" << endl;
// // //         }
// // //
// // //         cout << "\nCurrent Of voltage Source:" << endl;
// // //         for (const auto* vs_elem : circuit.getOrderedVoltageSources()) {
// // //             cout << "Current of " << vs_elem->getName() << ": " << vs_elem->getCurrent() << " A" << endl;
// // //         }
// // //
// // //         cout << "\nCurrent of Resistors: " << endl;
// // //         for (const auto* elem : circuit.getAllElements()) {
// // //            if (const Resistor* res = dynamic_cast<const Resistor*>(elem)) {
// // //                cout << "Current of " << res->getName() << " (" << res->getNode1()->getName() << "->" << res->getNode2()->getName() << "): "
// // //                     << res->getCurrent() << " A" << endl;
// // //            }
// // //         }
// // //
// // //
// // //     } catch (const std::exception& e) {
// // //         cerr << "OOps. " << e.what() << endl;
// // //     }
// // //
// // //     return 0;
// // // }
//
// // #include <iostream>
// // #include <string>
// // #include <vector>
// // #include <regex>
// // #include <stdexcept> // For logic_error
// // #include <algorithm> // For tolower
// // using namespace std;
// //
// // // Forward declaration of centralController
// // class centralController {
// // public:
// //     // Methods for section 13 & 18 (element/source creation/deletion)
// //     void makingResistor(const string& name, const string& node1, const string& node2, const string& value_str_raw) {
// //         cout << "Controller: Making Resistor: " << name << ", " << node1 << ", " << node2 << ", Value: " << value_str_raw << endl;
// //     }
// //     void deletingResistor(const string& name) {
// //         cout << "Controller: Deleting Resistor: " << name << endl;
// //     }
// //     void makingCapacity(const string& name, const string& node1, const string& node2, const string& value_str_raw) {
// //         cout << "Controller: Making Capacitor: " << name << ", " << node1 << ", " << node2 << ", Value: " << value_str_raw << endl;
// //     }
// //     void deleteCapacity(const string& name) {
// //         cout << "Controller: Deleting Capacitor: " << name << endl;
// //     }
// //     void makingInductor(const string& name, const string& node1, const string& node2, const string& value_str_raw) {
// //         cout << "Controller: Making Inductor: " << name << ", " << node1 << ", " << node2 << ", Value: " << value_str_raw << endl;
// //     }
// //     void deleteInductor(const string& name) {
// //         cout << "Controller: Deleting Inductor: " << name << endl;
// //     }
// //     void makingDiode(const string& name, const string& node1, const string& node2, const string& model) {
// //         cout << "Controller: Making Diode: " << name << ", " << node1 << ", " << node2 << ", Model: " << model << endl;
// //     }
// //     void deletingDiode(const string& name) {
// //         cout << "Controller: Deleting Diode: " << name << endl;
// //     }
// //     void addingGround(const string& node) {
// //         cout << "Controller: Adding Ground to node: " << node << endl;
// //     }
// //     void deletingGround(const string& node) {
// //         cout << "Controller: Deleting Ground from node: " << node << endl;
// //     }
// //
// //     // --- Source Creation/Deletion ---
// //     void makingVoltageSourceDC(const string& name, const string& node_plus, const string& node_minus, const string& value_str_raw) {
// //         cout << "Controller: Making DC Voltage Source: " << name << ", N+: " << node_plus << ", N-: " << node_minus << ", Value: " << value_str_raw << endl;
// //     }
// //     void makingCurrentSourceDC(const string& name, const string& node_plus, const string& node_minus, const string& value_str_raw) {
// //         cout << "Controller: Making DC Current Source: " << name << ", N+: " << node_plus << ", N-: " << node_minus << ", Value: " << value_str_raw << endl;
// //     }
// //     void makingVoltageSourceSin(const string& name, const string& node_plus, const string& node_minus, const string& v_offset, const string& v_amplitude, const string& freq) {
// //         cout << "Controller: Making Sinusoidal Voltage Source: " << name << ", N+: " << node_plus << ", N-: " << node_minus
// //              << ", Voffset: " << v_offset << ", Vamplitude: " << v_amplitude << ", Freq: " << freq << endl;
// //     }
// //     void makingVCVS(const string& name, const string& n_plus, const string& n_minus, const string& nc_plus, const string& nc_minus, const string& gain_str) {
// //         cout << "Controller: Making VCVS (E): " << name << " " << n_plus << " " << n_minus << " " << nc_plus << " " << nc_minus << " " << gain_str << endl;
// //     }
// //     void makingVCCS(const string& name, const string& n_plus, const string& n_minus, const string& nc_plus, const string& nc_minus, const string& gain_str) {
// //         cout << "Controller: Making VCCS (G): " << name << " " << n_plus << " " << n_minus << " " << nc_plus << " " << nc_minus << " " << gain_str << endl;
// //     }
// //     void makingCCVS(const string& name, const string& n_plus, const string& n_minus, const string& v_control_name, const string& gain_str) {
// //         cout << "Controller: Making CCVS (H): " << name << " " << n_plus << " " << n_minus << " " << v_control_name << " " << gain_str << endl;
// //     }
// //     void makingCCCS(const string& name, const string& n_plus, const string& n_minus, const string& v_control_name, const string& gain_str) {
// //         cout << "Controller: Making CCCS (F): " << name << " " << n_plus << " " << n_minus << " " << v_control_name << " " << gain_str << endl;
// //     }
// //     void deletingSource(const string& name) { // Generic delete for sources V, I, E, G, H, F
// //         cout << "Controller: Deleting Source: " << name << endl;
// //     }
// //
// //
// //     // Placeholder methods for section 14, 15, 17 commands
// //     void handleNodesCommand() {
// //         cout << "Controller: Handling 'nodes' command." << endl;
// //         cout << "Available nodes:\nn001, n002, VDD, GND, Vout, Vin" << endl; // PDF Example
// //     }
// //
// //     void handleListCommand(const string& componentType = "") {
// //         if (componentType.empty()) {
// //             cout << "Controller: Handling 'list' command (all components)." << endl;
// //         } else {
// //             cout << "Controller: Handling 'list " << componentType << "' command." << endl;
// //         }
// //     }
// //
// //     void handleRenameNodeCommand(const string& oldName, const string& newName) {
// //         // This method in controller would check for existence and then perform rename
// //         cout << "Controller: Attempting to rename node " << oldName << " to " << newName << "." << endl;
// //         // Simulate success based on PDF example
// //         cout << "SUCCESS: Node renamed from " << oldName << " to " << newName << endl;
// //         // Or throw specific errors based on checks:
// //         // throw logic_error("ERROR: Node " + oldName + " does not exist in the circuit");
// //         // throw logic_error("ERROR: Node name " + newName + " already exists");
// //     }
// //
// //     void handlePrintCommand(const string& analysisType, const string& printArgs) {
// //         cout << "Controller: Handling 'print " << analysisType << "' command with args: '" << printArgs << "'." << endl;
// //         // Check for errors like "Node <node_name> not found in circuit" or "Component <component_name> not found" here
// //     }
// //
// //     void handleNewFileCommand(const string& filePath) {
// //         cout << "Controller: Handling 'NewFile " << filePath << "' command." << endl;
// //     }
// //
// //     // --- Menu related state/handlers ---
// //     bool is_expecting_schematic_choice = false; // Example state variable
// //
// //     void handleShowExistingSchematicsCommand() {
// //         cout << "Controller: Handling 'show existing schematics' command." << endl;
// //         cout << "-choose existing schematic:\n1-draft1\n2-draft2\n3-draft3\n4-elecphase1" << endl; // PDF Example
// //         is_expecting_schematic_choice = true;
// //     }
// //
// //     void handleChooseSchematic(const string& choice) {
// //         is_expecting_schematic_choice = false; // Reset state
// //         if (choice == "return") {
// //             cout << "Controller: Returning to main menu..." << endl;
// //         } else {
// //             // Validate choice (e.g., is it a number within range?)
// //             bool isValidChoice = true; // Placeholder for actual validation
// //             for(char const &c : choice) { if (!std::isdigit(c)) isValidChoice = false; break;}
// //             // Based on PDF, if choice is invalid (e.g., "25", "rreturn", "draft1")
// //             if (!isValidChoice /* or choice out of bounds */ ) {
// //                  cout << "-Error: Inappropriate input" << endl; // PDF Error message
// //                  is_expecting_schematic_choice = true; // Remain in this state
// //                  return;
// //             }
// //             cout << "Controller: Loading schematic " << choice << "..." << endl;
// //             // ... display netlist ...
// //             cout << "draft" << choice << ":\nV1 in 0 DC O AC 1 SIN(0 1 1k)\n...\n.end" << endl; // Simplified example
// //             // According to PDF, after showing netlist, it returns to the "choose existing schematic" prompt
// //             handleShowExistingSchematicsCommand();
// //         }
// //     }
// // };
// //
// // // Helper functions (from user's .txt file, with minor improvements)
// // bool checkDouble(const string& s) {
// //     if (s.empty()) return false;
// //     size_t i = 0;
// //     bool negative = false;
// //     if (s[0] == '-') {
// //         if (s.length() == 1) return false; // Just "-"
// //         negative = true;
// //         i = 1;
// //     }
// //     bool digitFound = false;
// //     bool dotFound = false;
// //     for (; i < s.length(); ++i) {
// //         if (isdigit(s[i])) {
// //             digitFound = true;
// //         } else if (s[i] == '.') {
// //             if (dotFound) return false; // Multiple dots
// //             dotFound = true;
// //         } else {
// //             return false; // Invalid character
// //         }
// //     }
// //     if (!digitFound) return false; // No digits (e.g., ".", "-.")
// //     try {
// //         stod(s);
// //     } catch (const std::invalid_argument&) {
// //         return false;
// //     } catch (const std::out_of_range&) {
// //         return false;
// //     }
// //     return true;
// // }
// //
// // bool checkingNemadElmi(const string& s) {
// //     if (s.empty()) return false;
// //     string temp_s = s;
// //     std::transform(temp_s.begin(), temp_s.end(), temp_s.begin(), ::tolower); // Convert to lowercase for 'e'
// //
// //     size_t e_pos = temp_s.find('e');
// //     if (e_pos == string::npos) return false; // No 'e'
// //
// //     string base_str = temp_s.substr(0, e_pos);
// //     string exp_str = temp_s.substr(e_pos + 1);
// //
// //     if (base_str.empty() || exp_str.empty()) return false;
// //
// //     if (!checkDouble(base_str)) return false;
// //
// //     // Check exponent part (must be integer, possibly signed)
// //     size_t i = 0;
// //     if (exp_str[0] == '+' || exp_str[0] == '-') {
// //         if (exp_str.length() == 1) return false; // "e+" or "e-"
// //         i = 1;
// //     }
// //     if (i == exp_str.length()) return false; // "e" or "e+" with no digits after sign
// //
// //     for (; i < exp_str.length(); ++i) {
// //         if (!isdigit(exp_str[i])) return false;
// //     }
// //     // Try converting the whole string to double to catch out_of_range for the combined value
// //     try {
// //         stod(s);
// //     } catch (const std::out_of_range&) {
// //         return false;
// //     }
// //     return true;
// // }
// //
// // pair<string, string> valuate(const string& s_in) {
// //     string s = s_in;
// //     string value_str = s;
// //     string prefix_str = "";
// //
// //     if (s.empty()) return {s, ""};
// //
// //     // Handle common SI prefixes. Order matters for multi-char prefixes (e.g., "Meg" before "m").
// //     // Case-insensitive check for prefixes might be good, but PDF implies case sensitivity for values.
// //     // For now, stick to what's common in SPICE-like inputs.
// //     if (s.length() >= 3 && (s.substr(s.length() - 3) == "Meg" || s.substr(s.length() - 3) == "MEG")) {
// //         value_str = s.substr(0, s.length() - 3);
// //         prefix_str = "Meg";
// //     } else if (s.length() >= 2) { // Check for single character prefixes
// //         char last_char = s.back();
// //         string potential_val_str = s.substr(0, s.length() - 1);
// //         // Only treat as prefix if the remaining part is potentially a number
// //         if (!potential_val_str.empty() && (isdigit(potential_val_str.back()) || potential_val_str.back() == '.')) {
// //             switch (last_char) {
// //                 case 'G': prefix_str = "G"; value_str = potential_val_str; break; // Giga
// //                 // Meg handled above
// //                 case 'k': case 'K': prefix_str = "k"; value_str = potential_val_str; break; // kilo
// //                 case 'm': prefix_str = "m"; value_str = potential_val_str; break; // milli
// //                 case 'u': case 'U': prefix_str = "u"; value_str = potential_val_str; break; // micro (often u in SPICE)
// //                 case 'n': case 'N': prefix_str = "n"; value_str = potential_val_str; break; // nano
// //                 case 'p': case 'P': prefix_str = "p"; value_str = potential_val_str; break; // pico
// //                 case 'f': case 'F': prefix_str = "f"; value_str = potential_val_str; break; // femto
// //                 // No default, if not a prefix, value_str remains s and prefix_str remains ""
// //             }
// //         }
// //     }
// //     // If value_str is now empty, it means the input was likely just a prefix (e.g., "k")
// //     if (value_str.empty() && !prefix_str.empty()) {
// //         // This is an invalid format, e.g. "k" alone. Let checkDouble/NemadElmi catch it.
// //         return {s_in, ""}; // Return original and let validation fail
// //     }
// //     return {value_str, prefix_str};
// // }
// //
// //
// // class CommandParser {
// // private:
// //     centralController& controller;
// //
// //     // --- Element Parsing (Section 13) ---
// //     bool tryParseElementCommands(const string& in) {
// //         smatch matches;
// //         // Resistor
// //         regex add_res_pattern(R"(^add (R\w+) (\w+) (\w+) (.+?)$)"); // R<name> node1 node2 value
// //         regex del_res_pattern(R"(^delete (R\w+)$)");
// //         if (regex_match(in, matches, add_res_pattern)) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
// //             pair<string, string> p_val = valuate(val_raw);
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Resistance cannot be zero or negative"); // PDF 13.1.5.1 (adapted)
// //             if (stod(p_val.first) <= 0) throw logic_error("Error: Resistance cannot be zero or negative");
// //             controller.makingResistor(name, n1, n2, val_raw); return true;
// //         }
// //         if (regex_match(in, matches, del_res_pattern)) {
// //             controller.deletingResistor(matches[1].str()); return true;
// //         }
// //         // Capacitor
// //         regex add_cap_pattern(R"(^add (C\w+) (\w+) (\w+) (.+?)$)");
// //         regex del_cap_pattern(R"(^delete (C\w+)$)");
// //         if (regex_match(in, matches, add_cap_pattern)) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
// //             pair<string, string> p_val = valuate(val_raw);
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Capacitance cannot be zero or negative"); // PDF 13.2.5.1
// //             if (stod(p_val.first) <= 0) throw logic_error("Error: Capacitance cannot be zero or negative");
// //             controller.makingCapacity(name, n1, n2, val_raw); return true;
// //         }
// //         if (regex_match(in, matches, del_cap_pattern)) {
// //             controller.deleteCapacity(matches[1].str()); return true;
// //         }
// //         // Inductor
// //         regex add_ind_pattern(R"(^add (L\w+) (\w+) (\w+) (.+?)$)");
// //         regex del_ind_pattern(R"(^delete (L\w+)$)");
// //         if (regex_match(in, matches, add_ind_pattern)) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
// //             pair<string, string> p_val = valuate(val_raw);
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Inductance cannot be zero or negative"); // PDF 13.3.5.1
// //             if (stod(p_val.first) <= 0) throw logic_error("Error: Inductance cannot be zero or negative");
// //             controller.makingInductor(name, n1, n2, val_raw); return true;
// //         }
// //         if (regex_match(in, matches, del_ind_pattern)) {
// //             controller.deleteInductor(matches[1].str()); return true;
// //         }
// //         // Diode
// //         regex add_diode_pattern(R"(^add (D\w+) (\w+) (\w+) (D|Z)$)"); // Model D or Z
// //         regex del_diode_pattern(R"(^delete (D\w+)$)");
// //         if (regex_match(in, matches, add_diode_pattern)) {
// //             // Model validation is part of regex
// //             controller.makingDiode(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
// //         }
// //         if (regex_match(in, matches, del_diode_pattern)) {
// //             controller.deletingDiode(matches[1].str()); return true;
// //         }
// //         // Ground (PDF 13.5)
// //         regex add_gnd_pattern(R"(^add GND (\w+)$)"); // add GND <node>
// //         regex del_gnd_pattern(R"(^delete GND (\w+)$)"); // delete GND <node>
// //         if (regex_match(in, matches, add_gnd_pattern)) {
// //             controller.addingGround(matches[1].str()); return true;
// //         }
// //         if (regex_match(in, matches, del_gnd_pattern)) {
// //             // PDF 13.5.2: "if node does not exist, print 'Node does not exist'" - This check is for controller
// //             controller.deletingGround(matches[1].str()); return true;
// //         }
// //         return false;
// //     }
// //
// //     // --- Source Parsing (Section 18) ---
// //     bool tryParseSourceCommands(const string& in) {
// //         smatch matches;
// //         // Independent DC Voltage Source: add V<Name> <Node1> <Node2> <Value>
// //         regex add_vdc_pattern(R"(^add (V\w+) (\w+) (\w+) ([^\s(]+)$)"); // Value should not start with ( for SIN/PULSE
// //         if (regex_match(in, matches, add_vdc_pattern) && matches[4].str().find("SIN") == string::npos && matches[4].str().find("PULSE") == string::npos) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
// //             pair<string, string> p_val = valuate(val_raw);
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value for DC voltage source " + name);
// //             // DC value can be zero or negative
// //             controller.makingVoltageSourceDC(name, n1, n2, val_raw); return true;
// //         }
// //         // Independent DC Current Source: add I<Name> <Node1> <Node2> <Value>
// //         regex add_idc_pattern(R"(^add (I\w+) (\w+) (\w+) (.+?)$)");
// //         if (regex_match(in, matches, add_idc_pattern)) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
// //             pair<string, string> p_val = valuate(val_raw);
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value for DC current source " + name);
// //             controller.makingCurrentSourceDC(name, n1, n2, val_raw); return true;
// //         }
// //         // Sinusoidal Voltage Source: add V<name> <n+> <n-> SIN(<Voffset> <Vamplitude> <Frequency>) (PDF 18.2)
// //         // Regex needs to be careful with spaces around commas and parentheses.
// //         regex add_vsin_pattern(R"(^add (V\w+) (\w+) (\w+) SIN\s*\(\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*\)$)");
// //         if (regex_match(in, matches, add_vsin_pattern)) {
// //             string name=matches[1].str(), n_plus=matches[2].str(), n_minus=matches[3].str();
// //             string voff_raw=matches[4].str(), vamp_raw=matches[5].str(), freq_raw=matches[6].str();
// //             // Validate voff, vamp, freq (should be numbers)
// //             if (!checkDouble(valuate(voff_raw).first) && !checkingNemadElmi(valuate(voff_raw).first)) throw logic_error("Error: Invalid Voffset for SIN source " + name);
// //             if (!checkDouble(valuate(vamp_raw).first) && !checkingNemadElmi(valuate(vamp_raw).first)) throw logic_error("Error: Invalid Vamplitude for SIN source " + name);
// //             if (!checkDouble(valuate(freq_raw).first) && !checkingNemadElmi(valuate(freq_raw).first)) throw logic_error("Error: Invalid Frequency for SIN source " + name);
// //             if (stod(valuate(freq_raw).first) <= 0) throw logic_error("Error: Frequency for SIN source " + name + " must be positive.");
// //
// //             controller.makingVoltageSourceSin(name, n_plus, n_minus, voff_raw, vamp_raw, freq_raw); return true;
// //         }
// //
// //         // Dependent Sources (PDF 18.4)
// //         // VCVS (E): add E<Name> <N1> <N2> <CtrlN1> <CtrlN2> <Gain>
// //         regex add_vcvs_pattern(R"(^add (E\w+) (\w+) (\w+) (\w+) (\w+) (.+?)$)");
// //         if (regex_match(in, matches, add_vcvs_pattern)) {
// //             string gain_raw = matches[6].str();
// //             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain for VCVS " + matches[1].str());
// //             controller.makingVCVS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str(), gain_raw); return true;
// //         }
// //         // VCCS (G): add G<name> <n+> <n-> <control+> <control-> <Gain>
// //         regex add_vccs_pattern(R"(^add (G\w+) (\w+) (\w+) (\w+) (\w+) (.+?)$)");
// //         if (regex_match(in, matches, add_vccs_pattern)) {
// //             string gain_raw = matches[6].str();
// //             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain for VCCS " + matches[1].str());
// //             controller.makingVCCS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str(), gain_raw); return true;
// //         }
// //         // CCVS (H): add H<name> <n+> <n-> <vname> <gain> (vname is name of a voltage source whose current is controlling)
// //         regex add_ccvs_pattern(R"(^add (H\w+) (\w+) (\w+) (V\w+) (.+?)$)"); // Assuming vname starts with V
// //         if (regex_match(in, matches, add_ccvs_pattern)) {
// //             string gain_raw = matches[5].str();
// //             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain for CCVS " + matches[1].str());
// //             controller.makingCCVS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), gain_raw); return true;
// //         }
// //         // CCCS (F): add F<name> <n+> <n-> <vname> <gain>
// //         regex add_cccs_pattern(R"(^add (F\w+) (\w+) (\w+) (V\w+) (.+?)$)"); // Assuming vname starts with V
// //         if (regex_match(in, matches, add_cccs_pattern)) {
// //             string gain_raw = matches[5].str();
// //             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain for CCCS " + matches[1].str());
// //             controller.makingCCCS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), gain_raw); return true;
// //         }
// //
// //         // Generic delete for sources (V, I, E, G, H, F)
// //         regex del_src_pattern(R"(^delete ([VIEGHF]\w+)$)");
// //         if (regex_match(in, matches, del_src_pattern)) {
// //             controller.deletingSource(matches[1].str()); return true;
// //         }
// //         return false;
// //     }
// //
// //     // --- Node Operations (Section 14.1, 14.3) ---
// //     bool tryParseNodeCommands(const string& in) {
// //         smatch matches;
// //         // nodes (PDF 14.1)
// //         regex nodes_pattern(R"(^nodes$)");
// //         if (regex_match(in, matches, nodes_pattern)) {
// //             controller.handleNodesCommand(); return true;
// //         }
// //         // rename node <old_name> <new_name> (PDF 14.3)
// //         regex rename_node_base_pattern(R"(^rename node)");
// //         regex rename_node_full_pattern(R"(^rename node (\w+) (\w+)$)");
// //         if (regex_match(in, matches, rename_node_full_pattern)) {
// //             controller.handleRenameNodeCommand(matches[1].str(), matches[2].str()); return true;
// //         } else if (regex_search(in, rename_node_base_pattern)) { // Starts with "rename node" but not full match
// //             throw logic_error("ERROR: Invalid syntax correct format: rename node <old_name> <new_name>"); // PDF 14.3.7
// //         }
// //         return false;
// //     }
// //
// //     // --- Listing Operations (Section 14.2) ---
// //     bool tryParseListCommands(const string& in) {
// //         smatch matches;
// //         // list (PDF 14.2.1)
// //         regex list_all_pattern(R"(^list$)");
// //         if (regex_match(in, matches, list_all_pattern)) {
// //             controller.handleListCommand(); return true;
// //         }
// //         // list [component_type] (PDF 14.2) - Assuming component_type is a single letter like R, C, L, V, I etc.
// //         regex list_comp_pattern(R"(^list ([RCLDVIEGHF])$)"); // More specific component types
// //         if (regex_match(in, matches, list_comp_pattern)) {
// //             controller.handleListCommand(matches[1].str()); return true;
// //         }
// //         return false;
// //     }
// //
// //     // --- Print/Output Operations (Section 15) ---
// //     bool tryParsePrintCommands(const string& in) {
// //         smatch matches;
// //         // print <analysis_type> <variable1> ... (PDF 15.1)
// //         // Example: print TRAN <Tstep> <Tstop> [<Tstart>] [<Tmaxstep>] V(n001) (PDF 15.2)
// //         // Example: print DC <SrcName> <Start> <End> <Inc> V(out) (PDF 15.2)
// //         // This regex is generic; detailed parsing of args is for controller or analysis module
// //         regex print_pattern(R"(^print (TRAN|DC|AC) (.+)$)"); // AC added as typical
// //         if (regex_match(in, matches, print_pattern)) {
// //             try {
// //                 controller.handlePrintCommand(matches[1].str(), matches[2].str());
// //             } catch (const logic_error& e) { // Catch errors from controller like "Node not found"
// //                  // Check if the error message from controller matches PDF spec for print errors
// //                 string err_msg = e.what();
// //                 if (err_msg.find("not found in circuit") != string::npos) { // PDF 15.5.1, 15.5.2
// //                     throw; // Re-throw the specific error from controller
// //                 }
// //                 throw logic_error("Syntax error in command"); // PDF 15.5.3 generic syntax error
// //             }
// //             return true;
// //         }
// //         return false;
// //     }
// //
// //     // --- File and Menu Operations (Section 16, 17) ---
// //     bool tryParseFileAndMenuCommands(const string& in) {
// //         smatch matches;
// //         // NewFile <file_path> (PDF 17, after schematic list)
// //         regex new_file_pattern(R"(^NewFile\s+(.+)$)");
// //         if (regex_match(in, matches, new_file_pattern)) {
// //             controller.handleNewFileCommand(matches[1].str()); return true;
// //         }
// //         // show existing schematics (PDF 17)
// //         regex show_schematics_pattern(R"(^show existing schematics$)");
// //         if (regex_match(in, matches, show_schematics_pattern)) {
// //             controller.handleShowExistingSchematicsCommand(); return true;
// //         }
// //
// //         // Context-dependent input for choosing schematic (PDF 17)
// //         // This part is tricky as it depends on controller state
// //         if (controller.is_expecting_schematic_choice) {
// //             regex choose_num_pattern(R"(^(\d+)$)");
// //             regex choose_return_pattern(R"(^return$)");
// //             // Also handle invalid inputs like "draft1" directly as per PDF example
// //             regex choose_text_pattern(R"(^[a-zA-Z]+.*$)");
// //
// //
// //             if (regex_match(in, matches, choose_num_pattern)) {
// //                 controller.handleChooseSchematic(matches[1].str()); return true;
// //             } else if (regex_match(in, matches, choose_return_pattern)) {
// //                 controller.handleChooseSchematic("return"); return true;
// //             } else if (regex_match(in, matches, choose_text_pattern) || !in.empty()) {
// //                 // If it's not a number or "return", but something was typed (e.g. "draft1", "rreturn", "25")
// //                 // The controller's handleChooseSchematic should emit "-Error: Inappropriate input"
// //                 // and keep is_expecting_schematic_choice = true
// //                 controller.handleChooseSchematic(in); // Pass the raw invalid input
// //                 return true;
// //             }
// //         }
// //         return false;
// //     }
// //
// //
// // public:
// //     explicit CommandParser(centralController& ctrl) : controller(ctrl) {}
// //
// //     void processInput(const string& in) {
// //         if (in.empty()) return; // Ignore empty input
// //
// //         // Order of parsing attempts matters. More specific or context-dependent first.
// //         if (tryParseFileAndMenuCommands(in)) return; // Handles menu states
// //         if (tryParseNodeCommands(in)) return;
// //         if (tryParseListCommands(in)) return;
// //         if (tryParsePrintCommands(in)) return;
// //         if (tryParseElementCommands(in)) return;
// //         if (tryParseSourceCommands(in)) return; // Sources are also like elements
// //
// //         // If no command matched:
// //         // Check for common syntax errors for commands that might have been partially typed
// //         // e.g., "add" without enough arguments, or "delete" without enough.
// //         // The PDF provides specific syntax error messages for some commands (like rename node).
// //         // For others, a generic "Syntax error" or "Unknown command" might be appropriate.
// //         // The current structure throws specific errors from within tryParse methods if a base pattern matches but details are wrong.
// //
// //         // Final fallback for completely unrecognized commands
// //         // The PDF uses "Error: Syntax error" for some malformed element definitions (e.g. 13.1.5.2)
// //         // and "Syntax error in command" for print (15.5.3).
// //         // Let's use a general one.
// //         throw logic_error("Error: Unknown command or invalid syntax. -> " + in);
// //     }
// // };
// //
// // int main() {
// //     centralController controller_instance;
// //     CommandParser parser(controller_instance);
// //
// //     vector<string> test_commands = {
// //         // Section 14
// //         "nodes",
// //         "list",
// //         "list R",
// //         "list V",
// //         "rename node N001 Vout",
// //         "rename node N002", // Syntax Error
// //         // Section 15
// //         "print TRAN V(n001) I(R1)",
// //         "print DC Vsource 0 5 0.1 V(out)",
// //         // Section 17
// //         "show existing schematics", // Controller sets state
// //         "1",                      // To be handled by controller based on state
// //         "return",                 // To be handled by controller based on state
// //         "draft1",                 // Invalid choice, controller should show error
// //         "NewFile /path/to/circuit.txt",
// //         // Section 13 Elements
// //         "add R1 N1 N2 1k",
// //         "add Rload N_in N_out 2.2Meg",
// //         "add C1 N2 0 10u",
// //         "add L1 N1 N_intermediate 1mH",
// //         "add D1 N_intermediate N2 D",
// //         "add D_zener Vcc Vout Z",
// //         "delete R1",
// //         "add GND 0",
// //         "delete GND 0",
// //         "add RX N1 N2 0",     // Invalid value (caught by check)
// //         "add R2 N3 N4 -100",  // Invalid value (caught by check)
// //         "add R3 N5 N6 10p",
// //         "add R4 N7 N8 1.2e-3",
// //         "add R5 N9 N10 1.2e", // Invalid nemadElmi
// //         // Section 18 Sources
// //         "add Vsrc N_plus N_minus 5",
// //         "add Isrc N_in N_out 100m",
// //         "add Vosc P1 P2 SIN(0, 5, 1k)", // Test spaces in SIN
// //         "add Vpulse P3 P4 SIN( 1, 2.5, 500 )",
// //         "add Eamp Nout Nin Nctrl1 Nctrl2 100",
// //         "add Gtrans N_curr_out N_curr_in N_volt_ctrl1 N_volt_ctrl2 0.1",
// //         "add Hsense N_volt_out N_volt_gnd Vcontrol_current_src 50",
// //         "add Fmirror N_curr_out_2 N_curr_gnd_2 Vcontrol_current_src_for_F 1",
// //         "delete Vsrc",
// //         // Errors
// //         "add Rtest N1 N2 value_without_prefix", // Should be caught by checkDouble/NemadElmi
// //         "add Ctest N1 N2 100pF", // valuate should handle 'pF' if 'F' is added
// //         "add Ltest N1 N2 k", // valuate should return "k", then checkDouble fails
// //         "unknown command here",
// //         "add", // Incomplete
// //         "delete" // Incomplete
// //     };
// //
// //     cout << "--- Starting Command Tests ---" << endl;
// //     for (const string& cmd : test_commands) {
// //         cout << "\nInput: \"" << cmd << "\"" << endl;
// //         try {
// //             parser.processInput(cmd);
// //         } catch (const logic_error& e) {
// //             cerr << "Exception: " << e.what() << endl;
// //         }
// //     }
// //     cout << "\n--- Testing Schematic Choice Menu ---" << endl;
// //     string menu_inputs[] = {"show existing schematics", "5", "draft2", "1", "return"};
// //     for (const string& cmd : menu_inputs) {
// //         cout << "\nInput: \"" << cmd << "\"" << endl;
// //         try {
// //             parser.processInput(cmd);
// //         } catch (const logic_error& e) {
// //             cerr << "Exception: " << e.what() << endl;
// //         }
// //     }
// //
// //     return 0;
// //}
//
//
// ////////////////////////
//
// // #include <iostream>
// // #include <string>
// // #include <vector>
// // #include <regex>
// // #include <stdexcept> // For logic_error
// // #include <algorithm> // For tolower, transform
// // #include <iomanip>   // For stod precision issues if any, though not directly used for that here
// //
// // using namespace std;
// //
// // // Forward declaration of centralController
// // class centralController {
// // public:
// //     // Methods for section 13 & 18 (element/source creation/deletion)
// //     void makingResistor(const string& name, const string& node1, const string& node2, const string& value_str_raw) {
// //         cout << "Controller: Making Resistor: " << name << ", " << node1 << ", " << node2 << ", Value: " << value_str_raw << endl;
// //     }
// //     void deletingResistor(const string& name) {
// //         cout << "Controller: Deleting Resistor: " << name << endl;
// //     }
// //     void makingCapacity(const string& name, const string& node1, const string& node2, const string& value_str_raw) {
// //         cout << "Controller: Making Capacitor: " << name << ", " << node1 << ", " << node2 << ", Value: " << value_str_raw << endl;
// //     }
// //     void deleteCapacity(const string& name) {
// //         cout << "Controller: Deleting Capacitor: " << name << endl;
// //     }
// //     void makingInductor(const string& name, const string& node1, const string& node2, const string& value_str_raw) {
// //         cout << "Controller: Making Inductor: " << name << ", " << node1 << ", " << node2 << ", Value: " << value_str_raw << endl;
// //     }
// //     void deleteInductor(const string& name) {
// //         cout << "Controller: Deleting Inductor: " << name << endl;
// //     }
// //     void makingDiode(const string& name, const string& node1, const string& node2, const string& model) {
// //         cout << "Controller: Making Diode: " << name << ", " << node1 << ", " << node2 << ", Model: " << model << endl;
// //     }
// //     void deletingDiode(const string& name) {
// //         cout << "Controller: Deleting Diode: " << name << endl;
// //     }
// //     void addingGround(const string& node) {
// //         cout << "Controller: Adding Ground to node: " << node << endl;
// //     }
// //     void deletingGround(const string& node) {
// //         // As per PDF 13.5.2, this method in controller should check if node exists
// //         // and print "Node does not exist" if applicable.
// //         cout << "Controller: Deleting Ground from node: " << node << endl;
// //     }
// //
// //     // --- Source Creation/Deletion ---
// //     void makingVoltageSourceDC(const string& name, const string& node_plus, const string& node_minus, const string& value_str_raw) {
// //         cout << "Controller: Making DC Voltage Source: " << name << ", N+: " << node_plus << ", N-: " << node_minus << ", Value: " << value_str_raw << endl;
// //     }
// //     void makingCurrentSourceDC(const string& name, const string& node_plus, const string& node_minus, const string& value_str_raw) {
// //         cout << "Controller: Making DC Current Source: " << name << ", N+: " << node_plus << ", N-: " << node_minus << ", Value: " << value_str_raw << endl;
// //     }
// //     void makingVoltageSourceSin(const string& name, const string& node_plus, const string& node_minus, const string& v_offset, const string& v_amplitude, const string& freq) {
// //         cout << "Controller: Making Sinusoidal Voltage Source: " << name << ", N+: " << node_plus << ", N-: " << node_minus
// //              << ", Voffset: " << v_offset << ", Vamplitude: " << v_amplitude << ", Freq: " << freq << endl;
// //     }
// //     void makingVCVS(const string& name, const string& n_plus, const string& n_minus, const string& nc_plus, const string& nc_minus, const string& gain_str) {
// //         cout << "Controller: Making VCVS (E): " << name << " " << n_plus << " " << n_minus << " " << nc_plus << " " << nc_minus << " " << gain_str << endl;
// //     }
// //     void makingVCCS(const string& name, const string& n_plus, const string& n_minus, const string& nc_plus, const string& nc_minus, const string& gain_str) {
// //         cout << "Controller: Making VCCS (G): " << name << " " << n_plus << " " << n_minus << " " << nc_plus << " " << nc_minus << " " << gain_str << endl;
// //     }
// //     void makingCCVS(const string& name, const string& n_plus, const string& n_minus, const string& v_control_name, const string& gain_str) {
// //         cout << "Controller: Making CCVS (H): " << name << " " << n_plus << " " << n_minus << " " << v_control_name << " " << gain_str << endl;
// //     }
// //     void makingCCCS(const string& name, const string& n_plus, const string& n_minus, const string& v_control_name, const string& gain_str) {
// //         cout << "Controller: Making CCCS (F): " << name << " " << n_plus << " " << n_minus << " " << v_control_name << " " << gain_str << endl;
// //     }
// //     void deletingSource(const string& name) {
// //         cout << "Controller: Deleting Source: " << name << endl;
// //     }
// //
// //
// //     void handleNodesCommand() {
// //         cout << "Controller: Handling 'nodes' command." << endl;
// //         cout << "Available nodes:\nn001, n002, VDD, GND, Vout, Vin" << endl;
// //     }
// //
// //     void handleListCommand(const string& componentType = "") {
// //         if (componentType.empty()) {
// //             cout << "Controller: Handling 'list' command (all components)." << endl;
// //         } else {
// //             cout << "Controller: Handling 'list " << componentType << "' command." << endl;
// //         }
// //     }
// //
// //     void handleRenameNodeCommand(const string& oldName, const string& newName) {
// //         cout << "Controller: Attempting to rename node " << oldName << " to " << newName << "." << endl;
// //         // Simulate success based on PDF example for "rename node N001 Vout" (14.3.2)
// //         // Actual controller would check:
// //         // 1. if oldName exists (PDF 14.3.5: ERROR: Node <old_name> does not exist)
// //         // 2. if newName already exists (PDF 14.3.6: ERROR: Node name <new_name> already exists)
// //         cout << "SUCCESS: Node renamed from " << oldName << " to " << newName << endl;
// //     }
// //
// //     void handlePrintCommand(const string& analysisType, const string& printArgs) {
// //         cout << "Controller: Handling 'print " << analysisType << "' command with args: '" << printArgs << "'." << endl;
// //         // Controller would parse printArgs and check for errors like:
// //         // PDF 15.5.1: Node <node_name> not found in circuit
// //         // PDF 15.5.2: Component <component_name> not found in circuit
// //     }
// //
// //     void handleNewFileCommand(const string& filePath) {
// //         cout << "Controller: Handling 'NewFile " << filePath << "' command." << endl;
// //     }
// //
// //     bool is_expecting_schematic_choice = false;
// //
// //     void handleShowExistingSchematicsCommand() {
// //         cout << "Controller: Handling 'show existing schematics' command." << endl;
// //         cout << "-choose existing schematic:\n1-draft1\n2-draft2\n3-draft3\n4-elecphase1" << endl;
// //         is_expecting_schematic_choice = true;
// //     }
// //
// //     void handleChooseSchematic(const string& choice) {
// //         // This method is responsible for PDF 17's menu logic including error "-Error: Inappropriate input"
// //         cout << "Controller: Handling schematic choice: " << choice << endl;
// //
// //         if (choice == "return") {
// //             is_expecting_schematic_choice = false;
// //             cout << "Controller: Returning to main menu..." << endl;
// //             return;
// //         }
// //
// //         bool isValidNumericChoice = true;
// //         int chosen_num = 0;
// //         if (choice.empty() || !all_of(choice.begin(), choice.end(), ::isdigit)) {
// //             isValidNumericChoice = false;
// //         } else {
// //             try {
// //                 chosen_num = stoi(choice);
// //             } catch (const std::out_of_range&) {
// //                 isValidNumericChoice = false; // Number too large
// //             }
// //         }
// //
// //         // Example valid range based on PDF output
// //         if (isValidNumericChoice && chosen_num >= 1 && chosen_num <= 4) {
// //             is_expecting_schematic_choice = false; // Valid choice, reset state for now
// //             cout << "Controller: Loading schematic " << choice << "..." << endl;
// //             cout << "draft" << choice << ":\nV1 in 0 DC O AC 1 SIN(0 1 1k)\n...\n.end" << endl;
// //             // As per PDF, after showing netlist, it returns to the "choose existing schematic" prompt.
// //             handleShowExistingSchematicsCommand(); // This will set is_expecting_schematic_choice back to true
// //         } else {
// //             // Invalid choice (e.g., "5", "draft2", "rreturn" as per PDF examples of bad input)
// //             cout << "-Error: Inappropriate input" << endl; // PDF Error message (section 17)
// //             // is_expecting_schematic_choice remains true, so it prompts again implicitly
// //         }
// //     }
// // };
// //
// // // Helper functions
// // bool checkDouble(const string& s_in) {
// //     if (s_in.empty()) return false;
// //     string s = s_in; // Make a copy for potential modification if needed (not currently)
// //     size_t i = 0;
// //     if (s[0] == '-') {
// //         if (s.length() == 1) return false;
// //         i = 1;
// //     }
// //     bool digitFound = false;
// //     bool dotFound = false;
// //     bool hasCharAfterDot = false;
// //
// //     for (; i < s.length(); ++i) {
// //         if (isdigit(s[i])) {
// //             digitFound = true;
// //             if (dotFound) hasCharAfterDot = true;
// //         } else if (s[i] == '.') {
// //             if (dotFound) return false;
// //             dotFound = true;
// //         } else {
// //             return false;
// //         }
// //     }
// //     if (!digitFound) return false; // e.g., "." or "-."
// //     if (dotFound && !hasCharAfterDot && s.length() > 0 && s.back() == '.') {
// //         // handles cases like "1." or "-2." which stod might accept but we might want to reject if strict
// //         // For now, let stod decide. If stod accepts "1.", it's fine.
// //     }
// //     try {
// //         stod(s); // Check if convertible
// //     } catch (const std::invalid_argument&) {
// //         return false;
// //     } catch (const std::out_of_range&) {
// //         return false;
// //     }
// //     return true;
// // }
// //
// // bool checkingNemadElmi(const string& s_in) {
// //     if (s_in.empty()) return false;
// //     string s = s_in;
// //     std::transform(s.begin(), s.end(), s.begin(), ::tolower);
// //
// //     size_t e_pos = s.find('e');
// //     if (e_pos == string::npos || e_pos == 0 || e_pos == s.length() - 1) return false;
// //
// //     string base_str = s.substr(0, e_pos);
// //     string exp_str = s.substr(e_pos + 1);
// //
// //     if (base_str.empty() || exp_str.empty()) return false;
// //     if (!checkDouble(base_str)) return false; // Base must be a valid double
// //
// //     size_t i = 0;
// //     if (exp_str[0] == '+' || exp_str[0] == '-') {
// //         if (exp_str.length() == 1) return false;
// //         i = 1;
// //     }
// //     if (i == exp_str.length()) return false;
// //
// //     for (; i < exp_str.length(); ++i) {
// //         if (!isdigit(exp_str[i])) return false;
// //     }
// //     try {
// //         stod(s_in); // Check the original string for overall range
// //     } catch (const std::out_of_range&) {
// //         return false;
// //     }
// //     return true;
// // }
// //
// // pair<string, string> valuate(const string& s_in) {
// //     string s = s_in;
// //     string value_str = s;
// //     string prefix_str = "";
// //
// //     if (s.empty()) return {s, ""};
// //
// //     // Prefixes from PDF: k, Meg, u, n, m. Also common G, p, f.
// //     // Order matters for multi-char prefixes (e.g., "Meg" before "m").
// //     if (s.length() >= 3 && (s.substr(s.length() - 3) == "Meg" || s.substr(s.length() - 3) == "MEG")) {
// //         value_str = s.substr(0, s.length() - 3);
// //         prefix_str = "Meg";
// //     } else if (s.length() >= 2) {
// //         char last_char = s.back();
// //         string potential_val_str = s.substr(0, s.length() - 1);
// //         if (!potential_val_str.empty() && (isdigit(potential_val_str.back()) || potential_val_str.back() == '.' || (potential_val_str.length() == 1 && potential_val_str[0] == '-'))) {
// //             switch (last_char) {
// //                 case 'G': case 'g': prefix_str = "G"; value_str = potential_val_str; break;
// //                 case 'k': case 'K': prefix_str = "k"; value_str = potential_val_str; break;
// //                 case 'm': prefix_str = "m"; value_str = potential_val_str; break; // milli
// //                 case 'u': case 'U': prefix_str = "u"; value_str = potential_val_str; break;
// //                 case 'n': case 'N': prefix_str = "n"; value_str = potential_val_str; break;
// //                 case 'p': case 'P': prefix_str = "p"; value_str = potential_val_str; break;
// //                 case 'f': case 'F': prefix_str = "f"; value_str = potential_val_str; break;
// //                 // Note: 'F' for femto, not Farad. PDF uses '1u', not '1uF'.
// //             }
// //         }
// //     }
// //     if (value_str.empty() && !prefix_str.empty()) {
// //         return {s_in, ""};
// //     }
// //     // The PDF does not show units like 'H', 'F', 'Ohm' appended to the value string.
// //     // e.g., it's "1m", not "1mH". So, we don't strip those here.
// //     // If "1mH" is passed, value_str will be "1mH", which will fail checkDouble/NemadElmi.
// //     return {value_str, prefix_str};
// // }
// //
// //
// // class CommandParser {
// // private:
// //     centralController& controller;
// //
// //     // --- Element Parsing (Section 13) ---
// //     bool tryParseElementCommands(const string& in) {
// //         smatch matches;
// //         // Resistor
// //         regex add_res_pattern(R"(^add (R\w*) (\w+) (\w+) (.+?)$)");
// //         regex del_res_pattern(R"(^delete (R\w+)$)");
// //         if (regex_match(in, matches, add_res_pattern)) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
// //             pair<string, string> p_val = valuate(val_raw);
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for " + name);
// //             double num_val = stod(p_val.first);
// //             if (num_val <= 0) throw logic_error("Error: Resistance cannot be zero or negative"); // PDF 13.1.5.1
// //             controller.makingResistor(name, n1, n2, val_raw); return true;
// //         }
// //         if (regex_match(in, matches, del_res_pattern)) {
// //             controller.deletingResistor(matches[1].str()); return true;
// //         }
// //         // Capacitor
// //         regex add_cap_pattern(R"(^add (C\w+) (\w+) (\w+) (.+?)$)");
// //         regex del_cap_pattern(R"(^delete (C\w+)$)");
// //         if (regex_match(in, matches, add_cap_pattern)) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
// //             pair<string, string> p_val = valuate(val_raw);
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for " + name);
// //             double num_val = stod(p_val.first);
// //             if (num_val <= 0) throw logic_error("Error: Capacitance cannot be zero or negative"); // PDF 13.2.5.1
// //             controller.makingCapacity(name, n1, n2, val_raw); return true;
// //         }
// //         if (regex_match(in, matches, del_cap_pattern)) {
// //             controller.deleteCapacity(matches[1].str()); return true;
// //         }
// //         // Inductor
// //         regex add_ind_pattern(R"(^add (L\w+) (\w+) (\w+) (.+?)$)");
// //         regex del_ind_pattern(R"(^delete (L\w+)$)");
// //         if (regex_match(in, matches, add_ind_pattern)) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
// //             pair<string, string> p_val = valuate(val_raw);
// //             // If val_raw is "1mH", p_val.first will be "1mH". checkDouble("1mH") is false.
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for " + name);
// //             double num_val = stod(p_val.first);
// //             if (num_val <= 0) throw logic_error("Error: Inductance cannot be zero or negative"); // PDF 13.3.5.1
// //             controller.makingInductor(name, n1, n2, val_raw); return true;
// //         }
// //         if (regex_match(in, matches, del_ind_pattern)) {
// //             controller.deleteInductor(matches[1].str()); return true;
// //         }
// //         // Diode
// //         regex add_diode_pattern(R"(^add (D\w+) (\w+) (\w+) (D|Z)$)");
// //         regex del_diode_pattern(R"(^delete (D\w+)$)");
// //         if (regex_match(in, matches, add_diode_pattern)) {
// //             controller.makingDiode(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
// //         }
// //         if (regex_match(in, matches, del_diode_pattern)) {
// //             controller.deletingDiode(matches[1].str()); return true;
// //         }
// //         // Ground (PDF 13.5)
// //         regex add_gnd_pattern(R"(^add GND (\w+)$)");
// //         regex del_gnd_pattern(R"(^delete GND (\w+)$)");
// //         if (regex_match(in, matches, add_gnd_pattern)) {
// //             controller.addingGround(matches[1].str()); return true;
// //         }
// //         if (regex_match(in, matches, del_gnd_pattern)) {
// //             controller.deletingGround(matches[1].str()); return true;
// //         }
// //         return false;
// //     }
// //
// //     // --- Source Parsing (Section 18) ---
// //     bool tryParseSourceCommands(const string& in) {
// //         smatch matches;
// //         // Independent DC Voltage Source: add V<Name> <Node1> <Node2> <Value>
// //         // Value should not be SIN(...) or PULSE(...)
// //         regex add_vdc_pattern(R"(^add (V\w+) (\w+) (\w+) ([^S\s][^I\s][^N\s][^P\s][^U\s][^L\s][^S\s][^E\s].*?|[^-+0-9.]?\S+)$)");
// //         // A more robust way for VDC value: capture everything and check if it's NOT SIN/PULSE and IS a number.
// //         regex add_v_generic_pattern(R"(^add (V\w+) (\w+) (\w+) (.+?)$)");
// //
// //         if (regex_match(in, matches, add_v_generic_pattern)) {
// //             string name = matches[1].str();
// //             string n1 = matches[2].str();
// //             string n2 = matches[3].str();
// //             string val_full_str = matches[4].str();
// //
// //             // Check for SIN
// //             regex vsin_params_pattern(R"(^SIN\s*\(\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*\)$)");
// //             smatch sin_matches;
// //             if (regex_match(val_full_str, sin_matches, vsin_params_pattern)) {
// //                 string voff_raw=sin_matches[1].str(), vamp_raw=sin_matches[2].str(), freq_raw=sin_matches[3].str();
// //                 if (!checkDouble(valuate(voff_raw).first) && !checkingNemadElmi(valuate(voff_raw).first)) throw logic_error("Error: Invalid Voffset for SIN source " + name);
// //                 if (!checkDouble(valuate(vamp_raw).first) && !checkingNemadElmi(valuate(vamp_raw).first)) throw logic_error("Error: Invalid Vamplitude for SIN source " + name);
// //                 if (!checkDouble(valuate(freq_raw).first) && !checkingNemadElmi(valuate(freq_raw).first)) throw logic_error("Error: Invalid Frequency for SIN source " + name);
// //                 if (stod(valuate(freq_raw).first) <= 0) throw logic_error("Error: Frequency for SIN source " + name + " must be positive.");
// //                 controller.makingVoltageSourceSin(name, n1, n2, voff_raw, vamp_raw, freq_raw); return true;
// //             }
// //             // Add PULSE check here if implementing
// //             // else if (is PULSE) { ... }
// //
// //             // If not SIN or PULSE, assume DC
// //             else {
// //                 pair<string, string> p_val = valuate(val_full_str);
// //                 if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for DC voltage source " + name);
// //                 // DC value can be zero or negative, so no <=0 check here.
// //                 controller.makingVoltageSourceDC(name, n1, n2, val_full_str); return true;
// //             }
// //         }
// //
// //         // Independent DC Current Source: add I<Name> <Node1> <Node2> <Value>
// //         regex add_idc_pattern(R"(^add (I\w+) (\w+) (\w+) (.+?)$)");
// //         if (regex_match(in, matches, add_idc_pattern)) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
// //             pair<string, string> p_val = valuate(val_raw);
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for DC current source " + name);
// //             controller.makingCurrentSourceDC(name, n1, n2, val_raw); return true;
// //         }
// //
// //         // Dependent Sources (PDF 18.4)
// //         regex add_vcvs_pattern(R"(^add (E\w+) (\w+) (\w+) (\w+) (\w+) (.+?)$)");
// //         if (regex_match(in, matches, add_vcvs_pattern)) {
// //             string name=matches[1].str(), n_p=matches[2].str(), n_m=matches[3].str(), nc_p=matches[4].str(), nc_m=matches[5].str(), gain_raw=matches[6].str();
// //             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain format for VCVS " + name);
// //             controller.makingVCVS(name, n_p, n_m, nc_p, nc_m, gain_raw); return true;
// //         }
// //         regex add_vccs_pattern(R"(^add (G\w+) (\w+) (\w+) (\w+) (\w+) (.+?)$)");
// //         if (regex_match(in, matches, add_vccs_pattern)) {
// //             string name=matches[1].str(), n_p=matches[2].str(), n_m=matches[3].str(), nc_p=matches[4].str(), nc_m=matches[5].str(), gain_raw=matches[6].str();
// //             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain format for VCCS " + name);
// //             controller.makingVCCS(name, n_p, n_m, nc_p, nc_m, gain_raw); return true;
// //         }
// //         regex add_ccvs_pattern(R"(^add (H\w+) (\w+) (\w+) (V\w+) (.+?)$)");
// //         if (regex_match(in, matches, add_ccvs_pattern)) {
// //             string name=matches[1].str(), n_p=matches[2].str(), n_m=matches[3].str(), vctrl_name=matches[4].str(), gain_raw=matches[5].str();
// //             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain format for CCVS " + name);
// //             controller.makingCCVS(name, n_p, n_m, vctrl_name, gain_raw); return true;
// //         }
// //         regex add_cccs_pattern(R"(^add (F\w+) (\w+) (\w+) (V\w+) (.+?)$)");
// //         if (regex_match(in, matches, add_cccs_pattern)) {
// //             string name=matches[1].str(), n_p=matches[2].str(), n_m=matches[3].str(), vctrl_name=matches[4].str(), gain_raw=matches[5].str();
// //             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain format for CCCS " + name);
// //             controller.makingCCCS(name, n_p, n_m, vctrl_name, gain_raw); return true;
// //         }
// //
// //         regex del_src_pattern(R"(^delete ([VIEGHF]\w+)$)");
// //         if (regex_match(in, matches, del_src_pattern)) {
// //             controller.deletingSource(matches[1].str()); return true;
// //         }
// //         return false;
// //     }
// //
// //     bool tryParseNodeCommands(const string& in) {
// //         smatch matches;
// //         regex nodes_pattern(R"(^nodes$)");
// //         if (regex_match(in, matches, nodes_pattern)) {
// //             controller.handleNodesCommand(); return true;
// //         }
// //         regex rename_node_base_pattern(R"(^rename node)"); // For catching syntax error
// //         regex rename_node_full_pattern(R"(^rename node (\w+) (\w+)$)");
// //         if (regex_match(in, matches, rename_node_full_pattern)) {
// //             controller.handleRenameNodeCommand(matches[1].str(), matches[2].str()); return true;
// //         } else if (regex_search(in, rename_node_base_pattern)) {
// //             throw logic_error("ERROR: Invalid syntax correct format: rename node <old_name> <new_name>"); // PDF 14.3.7
// //         }
// //         return false;
// //     }
// //
// //     bool tryParseListCommands(const string& in) {
// //         smatch matches;
// //         regex list_all_pattern(R"(^list$)");
// //         if (regex_match(in, matches, list_all_pattern)) {
// //             controller.handleListCommand(); return true;
// //         }
// //         regex list_comp_pattern(R"(^list ([RCLDVIEGHF])$)");
// //         if (regex_match(in, matches, list_comp_pattern)) {
// //             controller.handleListCommand(matches[1].str()); return true;
// //         }
// //         return false;
// //     }
// //
// //     bool tryParsePrintCommands(const string& in) {
// //         smatch matches;
// //         regex print_pattern(R"(^print (TRAN|DC|AC) (.+)$)");
// //         if (regex_match(in, matches, print_pattern)) {
// //             try {
// //                 controller.handlePrintCommand(matches[1].str(), matches[2].str());
// //             } catch (const logic_error& e) {
// //                 string err_msg = e.what();
// //                 // Assuming controller throws errors that contain these substrings from PDF 15.5
// //                 if (err_msg.find("not found in circuit") != string::npos || err_msg.find("not found in circuit") != string::npos ) {
// //                     throw;
// //                 }
// //                 // If not a specific "not found" error, assume generic syntax error for print command
// //                 throw logic_error("Syntax error in command"); // PDF 15.5.3
// //             }
// //             return true;
// //         }
// //         return false;
// //     }
// //
// //     bool tryParseFileAndMenuCommands(const string& in) {
// //         smatch matches;
// //         regex new_file_pattern(R"(^NewFile\s+(.+)$)");
// //         if (regex_match(in, matches, new_file_pattern)) {
// //             controller.handleNewFileCommand(matches[1].str()); return true;
// //         }
// //         regex show_schematics_pattern(R"(^show existing schematics$)");
// //         if (regex_match(in, matches, show_schematics_pattern)) {
// //             controller.handleShowExistingSchematicsCommand(); return true;
// //         }
// //
// //         if (controller.is_expecting_schematic_choice) {
// //             // No specific regex here, pass directly to controller to handle all cases
// //             // (number, "return", or invalid text like "draft1", "rreturn")
// //             controller.handleChooseSchematic(in);
// //             return true;
// //         }
// //         return false;
// //     }
// //
// //
// // public:
// //     explicit CommandParser(centralController& ctrl) : controller(ctrl) {}
// //
// //     void processInput(const string& in) {
// //         if (in.empty()) {
// //             // cout << "Debug: Empty input ignored." << endl; // Optional debug
// //             return;
// //         }
// //         // cout << "Debug: Processing input: \"" << in << "\"" << endl; // Optional debug
// //
// //         // Order of parsing: Context-dependent (menu) first.
// //         if (tryParseFileAndMenuCommands(in)) return;
// //         // Then general utility commands
// //         if (tryParseNodeCommands(in)) return;
// //         if (tryParseListCommands(in)) return;
// //         if (tryParsePrintCommands(in)) return;
// //         // Then element/source definitions (often start with "add" or "delete")
// //         if (tryParseElementCommands(in)) return; // Handles R,C,L,D,GND
// //         if (tryParseSourceCommands(in)) return; // Handles V,I,E,G,H,F
// //
// //         // If no command matched:
// //         // PDF uses "Error: Syntax error" for malformed element def (e.g. 13.1.5.2)
// //         // PDF uses "Syntax error in command" for print (15.5.3).
// //         // For a completely unknown command, let's use a general message.
// //         throw logic_error("Error: Unknown command or invalid syntax. -> " + in);
// //     }
// // };
// //
// // int main() {
// //     centralController controller_instance;
// //     CommandParser parser(controller_instance);
// //
// //     vector<string> test_commands = {
// //         // Section 14
// //         "nodes",
// //         "list",
// //         "list R",
// //         "list V",
// //         "rename node N001 Vout",
// //         "rename node N002",
// //         // Section 15
// //         "print TRAN V(n001) I(R1)",
// //         "print DC Vsource 0 5 0.1 V(out)",
// //         // Section 17 (NewFile is here, show existing schematics is in menu_inputs)
// //         "NewFile /path/to/circuit.txt",
// //         // Section 13 Elements
// //         "add R1 N1 N2 1k",
// //         "add Rload N_in N_out 2.2Meg",
// //         "add C1 N2 0 10u",
// //         "add L1 N1 N_intermediate 1m", // Changed from 1mH to 1m to match PDF value format
// //         "add D1 N_intermediate N2 D",
// //         "add D_zener Vcc Vout Z",
// //         "delete R1",
// //         "add GND 0",
// //         "delete GND 0",
// //         "add RX N1 N2 0",
// //         "add R2 N3 N4 -100",
// //         "add R3 N5 N6 10p",
// //         "add R4 N7 N8 1.2e-3",
// //         "add R5 N9 N10 1.2e",
// //         // Section 18 Sources
// //         "add Vsrc N_plus N_minus 5",
// //         "add Isrc N_in N_out 100m",
// //         "add Vosc P1 P2 SIN(0, 5, 1k)",
// //         "add Vpulse P3 P4 SIN( 1, 2.5, 500 )",
// //         "add Eamp Nout Nin Nctrl1 Nctrl2 100",
// //         "add Gtrans N_curr_out N_curr_in N_volt_ctrl1 N_volt_ctrl2 0.1",
// //         "add Hsense N_volt_out N_volt_gnd Vcontrol_current_src 50",
// //         "add Fmirror N_curr_out_2 N_curr_gnd_2 Vcontrol_current_src_for_F 1",
// //         "delete Vsrc",
// //         // Errors
// //         "add Rtest N1 N2 value_without_prefix",
// //         "add Ctest N1 N2 100pF", // valuate should handle 'pF'. 'F' might be an issue if not stripped.
// //                                  // Current valuate expects 'p' and then checkDouble on "100".
// //         "add Ltest N1 N2 k",
// //         "add L_bad_unit N1 N2 1mH", // This should now cause "Invalid value format"
// //         "unknown command here",
// //         "add",
// //         "delete"
// //     };
// //
// //     cout << "--- Starting Command Tests ---" << endl;
// //     for (const string& cmd : test_commands) {
// //         cout << "\nInput: \"" << cmd << "\"" << endl;
// //         try {
// //             parser.processInput(cmd);
// //         } catch (const logic_error& e) {
// //             cerr << "Exception: " << e.what() << endl;
// //         }
// //     }
// //     cout << "\n--- Testing Schematic Choice Menu ---" << endl;
// //     // These inputs will be passed to controller.handleChooseSchematic
// //     // if controller.is_expecting_schematic_choice is true
// //     string menu_inputs[] = {"show existing schematics", "5", "draft2", "1", "return"};
// //     for (const string& cmd : menu_inputs) {
// //         cout << "\nInput: \"" << cmd << "\"" << endl;
// //         try {
// //             parser.processInput(cmd);
// //         } catch (const logic_error& e) {
// //             cerr << "Exception: " << e.what() << endl;
// //         }
// //     }
// //
// //     return 0;
// // }
//
// /////////////////////////////
// // #include <iostream>
// // #include <string>
// // #include <vector>
// // #include <regex>
// // #include <stdexcept> // For logic_error
// // #include <algorithm> // For tolower, transform
// // #include <iomanip>   // For stod precision issues if any, though not directly used for that here
// //
// // using namespace std;
// //
// // // Forward declaration of centralController
// // class centralController {
// // public:
// //     // Methods for section 13 & 18 (element/source creation/deletion)
// //     void makingResistor(const string& name, const string& node1, const string& node2, const string& value_str_raw) {
// //         cout << "Controller: Making Resistor: " << name << ", " << node1 << ", " << node2 << ", Value: " << value_str_raw << endl;
// //     }
// //     void deletingResistor(const string& name) {
// //         cout << "Controller: Deleting Resistor: " << name << endl;
// //     }
// //     void makingCapacity(const string& name, const string& node1, const string& node2, const string& value_str_raw) {
// //         cout << "Controller: Making Capacitor: " << name << ", " << node1 << ", " << node2 << ", Value: " << value_str_raw << endl;
// //     }
// //     void deleteCapacity(const string& name) {
// //         cout << "Controller: Deleting Capacitor: " << name << endl;
// //     }
// //     void makingInductor(const string& name, const string& node1, const string& node2, const string& value_str_raw) {
// //         cout << "Controller: Making Inductor: " << name << ", " << node1 << ", " << node2 << ", Value: " << value_str_raw << endl;
// //     }
// //     void deleteInductor(const string& name) {
// //         cout << "Controller: Deleting Inductor: " << name << endl;
// //     }
// //     void makingDiode(const string& name, const string& node1, const string& node2, const string& model) {
// //         cout << "Controller: Making Diode: " << name << ", " << node1 << ", " << node2 << ", Model: " << model << endl;
// //     }
// //     void deletingDiode(const string& name) {
// //         cout << "Controller: Deleting Diode: " << name << endl;
// //     }
// //     void addingGround(const string& node) {
// //         cout << "Controller: Adding Ground to node: " << node << endl;
// //     }
// //     void deletingGround(const string& node) {
// //         // As per PDF 13.5.2, this method in controller should check if node exists
// //         // and print "Node does not exist" if applicable.
// //         cout << "Controller: Deleting Ground from node: " << node << endl;
// //     }
// //
// //     // --- Source Creation/Deletion ---
// //     void makingVoltageSourceDC(const string& name, const string& node_plus, const string& node_minus, const string& value_str_raw) {
// //         cout << "Controller: Making DC Voltage Source: " << name << ", N+: " << node_plus << ", N-: " << node_minus << ", Value: " << value_str_raw << endl;
// //     }
// //     void makingCurrentSourceDC(const string& name, const string& node_plus, const string& node_minus, const string& value_str_raw) {
// //         cout << "Controller: Making DC Current Source: " << name << ", N+: " << node_plus << ", N-: " << node_minus << ", Value: " << value_str_raw << endl;
// //     }
// //     void makingVoltageSourceSin(const string& name, const string& node_plus, const string& node_minus, const string& v_offset, const string& v_amplitude, const string& freq) {
// //         cout << "Controller: Making Sinusoidal Voltage Source: " << name << ", N+: " << node_plus << ", N-: " << node_minus
// //              << ", Voffset: " << v_offset << ", Vamplitude: " << v_amplitude << ", Freq: " << freq << endl;
// //     }
// //     void makingVCVS(const string& name, const string& n_plus, const string& n_minus, const string& nc_plus, const string& nc_minus, const string& gain_str) {
// //         cout << "Controller: Making VCVS (E): " << name << " " << n_plus << " " << n_minus << " " << nc_plus << " " << nc_minus << " " << gain_str << endl;
// //     }
// //     void makingVCCS(const string& name, const string& n_plus, const string& n_minus, const string& nc_plus, const string& nc_minus, const string& gain_str) {
// //         cout << "Controller: Making VCCS (G): " << name << " " << n_plus << " " << n_minus << " " << nc_plus << " " << nc_minus << " " << gain_str << endl;
// //     }
// //     void makingCCVS(const string& name, const string& n_plus, const string& n_minus, const string& v_control_name, const string& gain_str) {
// //         cout << "Controller: Making CCVS (H): " << name << " " << n_plus << " " << n_minus << " " << v_control_name << " " << gain_str << endl;
// //     }
// //     void makingCCCS(const string& name, const string& n_plus, const string& n_minus, const string& v_control_name, const string& gain_str) {
// //         cout << "Controller: Making CCCS (F): " << name << " " << n_plus << " " << n_minus << " " << v_control_name << " " << gain_str << endl;
// //     }
// //     void deletingSource(const string& name) {
// //         cout << "Controller: Deleting Source: " << name << endl;
// //     }
// //
// //
// //     void handleNodesCommand() {
// //         cout << "Controller: Handling 'nodes' command." << endl;
// //         cout << "Available nodes:\nn001, n002, VDD, GND, Vout, Vin" << endl;
// //     }
// //
// //     void handleListCommand(const string& componentType = "") {
// //         if (componentType.empty()) {
// //             cout << "Controller: Handling 'list' command (all components)." << endl;
// //         } else {
// //             cout << "Controller: Handling 'list " << componentType << "' command." << endl;
// //         }
// //     }
// //
// //     void handleRenameNodeCommand(const string& oldName, const string& newName) {
// //         cout << "Controller: Attempting to rename node " << oldName << " to " << newName << "." << endl;
// //         // Simulate success based on PDF example for "rename node N001 Vout" (14.3.2)
// //         // Actual controller would check:
// //         // 1. if oldName exists (PDF 14.3.5: ERROR: Node <old_name> does not exist)
// //         // 2. if newName already exists (PDF 14.3.6: ERROR: Node name <new_name> already exists)
// //         cout << "SUCCESS: Node renamed from " << oldName << " to " << newName << endl;
// //     }
// //
// //     void handlePrintCommand(const string& analysisType, const string& printArgs) {
// //         cout << "Controller: Handling 'print " << analysisType << "' command with args: '" << printArgs << "'." << endl;
// //         // Controller would parse printArgs and check for errors like:
// //         // PDF 15.5.1: Node <node_name> not found in circuit
// //         // PDF 15.5.2: Component <component_name> not found in circuit
// //     }
// //
// //     void handleNewFileCommand(const string& filePath) {
// //         cout << "Controller: Handling 'NewFile " << filePath << "' command." << endl;
// //     }
// //
// //     bool is_expecting_schematic_choice = false;
// //
// //     void handleShowExistingSchematicsCommand() {
// //         cout << "Controller: Handling 'show existing schematics' command." << endl;
// //         cout << "-choose existing schematic:\n1-draft1\n2-draft2\n3-draft3\n4-elecphase1" << endl;
// //         is_expecting_schematic_choice = true;
// //     }
// //
// //     void handleChooseSchematic(const string& choice) {
// //         // This method is responsible for PDF 17's menu logic including error "-Error: Inappropriate input"
// //         cout << "Controller: Handling schematic choice: " << choice << endl;
// //
// //         if (choice == "return") {
// //             is_expecting_schematic_choice = false;
// //             cout << "Controller: Returning to main menu..." << endl;
// //             return;
// //         }
// //
// //         bool isValidNumericChoice = true;
// //         int chosen_num = 0;
// //         if (choice.empty() || !all_of(choice.begin(), choice.end(), ::isdigit)) {
// //             isValidNumericChoice = false;
// //         } else {
// //             try {
// //                 chosen_num = stoi(choice);
// //             } catch (const std::out_of_range&) {
// //                 isValidNumericChoice = false; // Number too large
// //             }
// //         }
// //
// //         // Example valid range based on PDF output
// //         if (isValidNumericChoice && chosen_num >= 1 && chosen_num <= 4) {
// //             is_expecting_schematic_choice = false; // Valid choice, reset state for now
// //             cout << "Controller: Loading schematic " << choice << "..." << endl;
// //             cout << "draft" << choice << ":\nV1 in 0 DC O AC 1 SIN(0 1 1k)\n...\n.end" << endl;
// //             // As per PDF, after showing netlist, it returns to the "choose existing schematic" prompt.
// //             handleShowExistingSchematicsCommand(); // This will set is_expecting_schematic_choice back to true
// //         } else {
// //             // Invalid choice (e.g., "5", "draft2", "rreturn" as per PDF examples of bad input)
// //             cout << "-Error: Inappropriate input" << endl; // PDF Error message (section 17)
// //             // is_expecting_schematic_choice remains true, so it prompts again implicitly
// //         }
// //     }
// // };
// //
// // // Helper functions
// // bool checkDouble(const string& s_in) {
// //     if (s_in.empty()) return false;
// //     string s = s_in;
// //     size_t i = 0;
// //     if (s[0] == '-') {
// //         if (s.length() == 1) return false;
// //         i = 1;
// //     }
// //     bool digitFound = false;
// //     bool dotFound = false;
// //     bool hasCharAfterDot = false;
// //
// //     for (; i < s.length(); ++i) {
// //         if (isdigit(s[i])) {
// //             digitFound = true;
// //             if (dotFound) hasCharAfterDot = true;
// //         } else if (s[i] == '.') {
// //             if (dotFound) return false;
// //             dotFound = true;
// //         } else {
// //             return false;
// //         }
// //     }
// //     if (!digitFound) return false;
// //     if (dotFound && !hasCharAfterDot && s.length() > 0 && s.back() == '.') {
// //         // For now, let stod decide. If stod accepts "1.", it's fine.
// //     }
// //     try {
// //         stod(s);
// //     } catch (const std::invalid_argument&) {
// //         return false;
// //     } catch (const std::out_of_range&) {
// //         return false;
// //     }
// //     return true;
// // }
// //
// // bool checkingNemadElmi(const string& s_in) {
// //     if (s_in.empty()) return false;
// //     string s = s_in;
// //     std::transform(s.begin(), s.end(), s.begin(), ::tolower);
// //
// //     size_t e_pos = s.find('e');
// //     if (e_pos == string::npos || e_pos == 0 || e_pos == s.length() - 1) return false;
// //
// //     string base_str = s.substr(0, e_pos);
// //     string exp_str = s.substr(e_pos + 1);
// //
// //     if (base_str.empty() || exp_str.empty()) return false;
// //     if (!checkDouble(base_str)) return false;
// //
// //     size_t i = 0;
// //     if (exp_str[0] == '+' || exp_str[0] == '-') {
// //         if (exp_str.length() == 1) return false;
// //         i = 1;
// //     }
// //     if (i == exp_str.length()) return false;
// //
// //     for (; i < exp_str.length(); ++i) {
// //         if (!isdigit(exp_str[i])) return false;
// //     }
// //     try {
// //         stod(s_in);
// //     } catch (const std::out_of_range&) {
// //         return false;
// //     }
// //     return true;
// // }
// //
// // pair<string, string> valuate(const string& s_in) {
// //     string s = s_in;
// //     string value_str = s;
// //     string prefix_str = "";
// //
// //     if (s.empty()) return {s, ""};
// //
// //     if (s.length() >= 3 && (s.substr(s.length() - 3) == "Meg" || s.substr(s.length() - 3) == "MEG")) {
// //         value_str = s.substr(0, s.length() - 3);
// //         prefix_str = "Meg";
// //     } else if (s.length() >= 2) {
// //         char last_char = s.back();
// //         string potential_val_str = s.substr(0, s.length() - 1);
// //         if (!potential_val_str.empty() && (isdigit(potential_val_str.back()) || potential_val_str.back() == '.' || (potential_val_str.length() == 1 && potential_val_str[0] == '-'))) {
// //             switch (last_char) {
// //                 case 'G': case 'g': prefix_str = "G"; value_str = potential_val_str; break;
// //                 case 'k': case 'K': prefix_str = "k"; value_str = potential_val_str; break;
// //                 case 'm': prefix_str = "m"; value_str = potential_val_str; break;
// //                 case 'u': case 'U': prefix_str = "u"; value_str = potential_val_str; break;
// //                 case 'n': case 'N': prefix_str = "n"; value_str = potential_val_str; break;
// //                 case 'p': case 'P': prefix_str = "p"; value_str = potential_val_str; break;
// //                 case 'f': case 'F': prefix_str = "f"; value_str = potential_val_str; break;
// //             }
// //         }
// //     }
// //     if (value_str.empty() && !prefix_str.empty()) {
// //         return {s_in, ""};
// //     }
// //     return {value_str, prefix_str};
// // }
// //
// //
// // class CommandParser {
// // private:
// //     centralController& controller;
// //
// //     bool tryParseElementCommands(const string& in) {
// //         smatch matches;
// //         regex add_res_pattern(R"(^add (R\w*) (\w+) (\w+) (.+?)$)");
// //         regex del_res_pattern(R"(^delete (R\w+)$)");
// //         if (regex_match(in, matches, add_res_pattern)) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
// //             pair<string, string> p_val = valuate(val_raw);
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for " + name);
// //             double num_val = stod(p_val.first);
// //             if (num_val <= 0) throw logic_error("Error: Resistance cannot be zero or negative");
// //             controller.makingResistor(name, n1, n2, val_raw); return true;
// //         }
// //         if (regex_match(in, matches, del_res_pattern)) {
// //             controller.deletingResistor(matches[1].str()); return true;
// //         }
// //         regex add_cap_pattern(R"(^add (C\w+) (\w+) (\w+) (.+?)$)");
// //         regex del_cap_pattern(R"(^delete (C\w+)$)");
// //         if (regex_match(in, matches, add_cap_pattern)) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
// //             pair<string, string> p_val = valuate(val_raw);
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for " + name);
// //             double num_val = stod(p_val.first);
// //             if (num_val <= 0) throw logic_error("Error: Capacitance cannot be zero or negative");
// //             controller.makingCapacity(name, n1, n2, val_raw); return true;
// //         }
// //         if (regex_match(in, matches, del_cap_pattern)) {
// //             controller.deleteCapacity(matches[1].str()); return true;
// //         }
// //         regex add_ind_pattern(R"(^add (L\w+) (\w+) (\w+) (.+?)$)");
// //         regex del_ind_pattern(R"(^delete (L\w+)$)");
// //         if (regex_match(in, matches, add_ind_pattern)) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
// //             pair<string, string> p_val = valuate(val_raw);
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for " + name);
// //             double num_val = stod(p_val.first);
// //             if (num_val <= 0) throw logic_error("Error: Inductance cannot be zero or negative");
// //             controller.makingInductor(name, n1, n2, val_raw); return true;
// //         }
// //         if (regex_match(in, matches, del_ind_pattern)) {
// //             controller.deleteInductor(matches[1].str()); return true;
// //         }
// //         regex add_diode_pattern(R"(^add (D\w+) (\w+) (\w+) (D|Z)$)");
// //         regex del_diode_pattern(R"(^delete (D\w+)$)");
// //         if (regex_match(in, matches, add_diode_pattern)) {
// //             controller.makingDiode(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
// //         }
// //         if (regex_match(in, matches, del_diode_pattern)) {
// //             controller.deletingDiode(matches[1].str()); return true;
// //         }
// //         regex add_gnd_pattern(R"(^add GND (\w+)$)");
// //         regex del_gnd_pattern(R"(^delete GND (\w+)$)");
// //         if (regex_match(in, matches, add_gnd_pattern)) {
// //             controller.addingGround(matches[1].str()); return true;
// //         }
// //         if (regex_match(in, matches, del_gnd_pattern)) {
// //             controller.deletingGround(matches[1].str()); return true;
// //         }
// //         return false;
// //     }
// //
// //     bool tryParseSourceCommands(const string& in) {
// //         smatch matches;
// //         regex add_v_generic_pattern(R"(^add (V\w+) (\w+) (\w+) (.+?)$)");
// //
// //         if (regex_match(in, matches, add_v_generic_pattern)) {
// //             string name = matches[1].str();
// //             string n1 = matches[2].str();
// //             string n2 = matches[3].str();
// //             string val_full_str = matches[4].str();
// //
// //             regex vsin_params_pattern(R"(^SIN\s*\(\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*\)$)");
// //             smatch sin_matches;
// //             if (regex_match(val_full_str, sin_matches, vsin_params_pattern)) {
// //                 string voff_raw=sin_matches[1].str(), vamp_raw=sin_matches[2].str(), freq_raw=sin_matches[3].str();
// //                 if (!checkDouble(valuate(voff_raw).first) && !checkingNemadElmi(valuate(voff_raw).first)) throw logic_error("Error: Invalid Voffset for SIN source " + name);
// //                 if (!checkDouble(valuate(vamp_raw).first) && !checkingNemadElmi(valuate(vamp_raw).first)) throw logic_error("Error: Invalid Vamplitude for SIN source " + name);
// //                 if (!checkDouble(valuate(freq_raw).first) && !checkingNemadElmi(valuate(freq_raw).first)) throw logic_error("Error: Invalid Frequency for SIN source " + name);
// //                 if (stod(valuate(freq_raw).first) <= 0) throw logic_error("Error: Frequency for SIN source " + name + " must be positive.");
// //                 controller.makingVoltageSourceSin(name, n1, n2, voff_raw, vamp_raw, freq_raw); return true;
// //             }
// //             else {
// //                 pair<string, string> p_val = valuate(val_full_str);
// //                 if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for DC voltage source " + name);
// //                 controller.makingVoltageSourceDC(name, n1, n2, val_full_str); return true;
// //             }
// //         }
// //
// //         regex add_idc_pattern(R"(^add (I\w+) (\w+) (\w+) (.+?)$)");
// //         if (regex_match(in, matches, add_idc_pattern)) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
// //             pair<string, string> p_val = valuate(val_raw);
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for DC current source " + name);
// //             controller.makingCurrentSourceDC(name, n1, n2, val_raw); return true;
// //         }
// //
// //         regex add_vcvs_pattern(R"(^add (E\w+) (\w+) (\w+) (\w+) (\w+) (.+?)$)");
// //         if (regex_match(in, matches, add_vcvs_pattern)) {
// //             string name=matches[1].str(), n_p=matches[2].str(), n_m=matches[3].str(), nc_p=matches[4].str(), nc_m=matches[5].str(), gain_raw=matches[6].str();
// //             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain format for VCVS " + name);
// //             controller.makingVCVS(name, n_p, n_m, nc_p, nc_m, gain_raw); return true;
// //         }
// //         regex add_vccs_pattern(R"(^add (G\w+) (\w+) (\w+) (\w+) (\w+) (.+?)$)");
// //         if (regex_match(in, matches, add_vccs_pattern)) {
// //             string name=matches[1].str(), n_p=matches[2].str(), n_m=matches[3].str(), nc_p=matches[4].str(), nc_m=matches[5].str(), gain_raw=matches[6].str();
// //             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain format for VCCS " + name);
// //             controller.makingVCCS(name, n_p, n_m, nc_p, nc_m, gain_raw); return true;
// //         }
// //         regex add_ccvs_pattern(R"(^add (H\w+) (\w+) (\w+) (V\w+) (.+?)$)");
// //         if (regex_match(in, matches, add_ccvs_pattern)) {
// //             string name=matches[1].str(), n_p=matches[2].str(), n_m=matches[3].str(), vctrl_name=matches[4].str(), gain_raw=matches[5].str();
// //             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain format for CCVS " + name);
// //             controller.makingCCVS(name, n_p, n_m, vctrl_name, gain_raw); return true;
// //         }
// //         regex add_cccs_pattern(R"(^add (F\w+) (\w+) (\w+) (V\w+) (.+?)$)");
// //         if (regex_match(in, matches, add_cccs_pattern)) {
// //             string name=matches[1].str(), n_p=matches[2].str(), n_m=matches[3].str(), vctrl_name=matches[4].str(), gain_raw=matches[5].str();
// //             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain format for CCCS " + name);
// //             controller.makingCCCS(name, n_p, n_m, vctrl_name, gain_raw); return true;
// //         }
// //
// //         regex del_src_pattern(R"(^delete ([VIEGHF]\w+)$)");
// //         if (regex_match(in, matches, del_src_pattern)) {
// //             controller.deletingSource(matches[1].str()); return true;
// //         }
// //         return false;
// //     }
// //
// //     bool tryParseNodeCommands(const string& in) {
// //         smatch matches;
// //         regex nodes_pattern(R"(^nodes$)");
// //         if (regex_match(in, matches, nodes_pattern)) {
// //             controller.handleNodesCommand(); return true;
// //         }
// //         regex rename_node_base_pattern(R"(^rename node)");
// //         regex rename_node_full_pattern(R"(^rename node (\w+) (\w+)$)");
// //         if (regex_match(in, matches, rename_node_full_pattern)) {
// //             controller.handleRenameNodeCommand(matches[1].str(), matches[2].str()); return true;
// //         } else if (regex_search(in, rename_node_base_pattern)) {
// //             throw logic_error("ERROR: Invalid syntax correct format: rename node <old_name> <new_name>");
// //         }
// //         return false;
// //     }
// //
// //     bool tryParseListCommands(const string& in) {
// //         smatch matches;
// //         regex list_all_pattern(R"(^list$)");
// //         if (regex_match(in, matches, list_all_pattern)) {
// //             controller.handleListCommand(); return true;
// //         }
// //         regex list_comp_pattern(R"(^list ([RCLDVIEGHF])$)");
// //         if (regex_match(in, matches, list_comp_pattern)) {
// //             controller.handleListCommand(matches[1].str()); return true;
// //         }
// //         return false;
// //     }
// //
// //     bool tryParsePrintCommands(const string& in) {
// //         smatch matches;
// //         regex print_pattern(R"(^print (TRAN|DC|AC) (.+)$)");
// //         if (regex_match(in, matches, print_pattern)) {
// //             try {
// //                 controller.handlePrintCommand(matches[1].str(), matches[2].str());
// //             } catch (const logic_error& e) {
// //                 string err_msg = e.what();
// //                 if (err_msg.find("not found in circuit") != string::npos || err_msg.find("not found in circuit") != string::npos ) {
// //                     throw;
// //                 }
// //                 throw logic_error("Syntax error in command");
// //             }
// //             return true;
// //         }
// //         return false;
// //     }
// //
// //     bool tryParseFileAndMenuCommands(const string& in) {
// //         smatch matches;
// //         regex new_file_pattern(R"(^NewFile\s+(.+)$)");
// //         if (regex_match(in, matches, new_file_pattern)) {
// //             controller.handleNewFileCommand(matches[1].str()); return true;
// //         }
// //         regex show_schematics_pattern(R"(^show existing schematics$)");
// //         if (regex_match(in, matches, show_schematics_pattern)) {
// //             controller.handleShowExistingSchematicsCommand(); return true;
// //         }
// //
// //         if (controller.is_expecting_schematic_choice) {
// //             controller.handleChooseSchematic(in);
// //             return true;
// //         }
// //         return false;
// //     }
// //
// //
// // public:
// //     explicit CommandParser(centralController& ctrl) : controller(ctrl) {}
// //
// //     void processInput(const string& in) {
// //         if (in.empty()) {
// //             return;
// //         }
// //
// //         if (tryParseFileAndMenuCommands(in)) return;
// //         if (tryParseNodeCommands(in)) return;
// //         if (tryParseListCommands(in)) return;
// //         if (tryParsePrintCommands(in)) return;
// //         if (tryParseElementCommands(in)) return;
// //         if (tryParseSourceCommands(in)) return;
// //
// //         throw logic_error("Error: Unknown command or invalid syntax. -> " + in);
// //     }
// // };
// //
// // int main() {
// //     centralController controller_instance;
// //     CommandParser parser(controller_instance);
// //
// //     vector<string> test_commands = {
// //         // Section 14
// //         "nodes",
// //         "list",
// //         "list R",
// //         "list V",
// //         "rename node N001 Vout",
// //         "rename node N002",
// //         // Section 15
// //         "print TRAN V(n001) I(R1)",
// //         "print DC Vsource 0 5 0.1 V(out)",
// //         // Section 17 (NewFile is here, show existing schematics is in menu_inputs)
// //         "NewFile /path/to/circuit.txt",
// //         // Section 13 Elements
// //         "add R1 N1 N2 1k",
// //         "add Rload N_in N_out 2.2Meg",
// //         "add C1 N2 0 10u",
// //         "add L1 N1 N_intermediate 1m",
// //         "add D1 N_intermediate N2 D",
// //         "add D_zener Vcc Vout Z",
// //         "delete R1",
// //         "add GND 0",
// //         "delete GND 0",
// //         "add RX N1 N2 0",
// //         "add R2 N3 N4 -100",
// //         "add R3 N5 N6 10p",
// //         "add R4 N7 N8 1.2e-3",
// //         "add R5 N9 N10 1.2e",
// //         // Section 18 Sources
// //         "add Vsrc N_plus N_minus 5",
// //         "add Isrc N_in N_out 100m",
// //         "add Vosc P1 P2 SIN(0, 5, 1k)",
// //         "add Vpulse P3 P4 SIN( 1, 2.5, 500 )",
// //         "add Eamp Nout Nin Nctrl1 Nctrl2 100",
// //         "add Gtrans N_curr_out N_curr_in N_volt_ctrl1 N_volt_ctrl2 0.1",
// //         "add Hsense N_volt_out N_volt_gnd Vcontrol_current_src 50",
// //         "add Fmirror N_curr_out_2 N_curr_gnd_2 Vcontrol_current_src_for_F 1",
// //         "delete Vsrc",
// //         // Errors
// //         "add Rtest N1 N2 value_without_prefix",
// //         "add Ctest N1 N2 100pF",
// //         "add Ltest N1 N2 k",
// //         "add L_bad_unit N1 N2 1mH",
// //         "unknown command here",
// //         "add",
// //         "delete"
// //     };
// //
// //     cout << "--- Starting Command Tests ---" << endl;
// //     for (const string& cmd : test_commands) {
// //         cout << "\nInput: \"" << cmd << "\"" << endl;
// //         try {
// //             parser.processInput(cmd);
// //         } catch (const logic_error& e) {
// //             // Changed cerr to cout for synchronized output
// //             cout << "Exception: " << e.what() << endl;
// //         }
// //     }
// //     cout << "\n--- Testing Schematic Choice Menu ---" << endl;
// //     string menu_inputs[] = {"show existing schematics", "5", "draft2", "1", "return"};
// //     for (const string& cmd : menu_inputs) {
// //         cout << "\nInput: \"" << cmd << "\"" << endl;
// //         try {
// //             parser.processInput(cmd);
// //         } catch (const logic_error& e) {
// //             // Changed cerr to cout for synchronized output
// //             cout << "Exception: " << e.what() << endl;
// //         }
// //     }
// //
// //     return 0;
// // }
// //
// // #include <iostream>
// // #include <string>
// // #include <vector>
// // #include <regex>
// // #include <stdexcept> // For logic_error
// // #include <algorithm> // For tolower, transform
// // #include <iomanip>   // For stod precision issues if any, though not directly used for that here
// //
// // using namespace std;
// //
// // // Forward declaration of centralController
// // class centralController {
// // public:
// //     // --- Elements ---
// //     void makingResistor(const string& name, const string& node1, const string& node2, const string& value_str_raw) {
// //         cout << "Controller: Making Resistor: " << name << ", " << node1 << ", " << node2 << ", Value: " << value_str_raw << endl;
// //     }
// //     void deletingResistor(const string& name) {
// //         cout << "Controller: Deleting Resistor: " << name << endl;
// //     }
// //     void makingCapacity(const string& name, const string& node1, const string& node2, const string& value_str_raw) {
// //         cout << "Controller: Making Capacitor: " << name << ", " << node1 << ", " << node2 << ", Value: " << value_str_raw << endl;
// //     }
// //     void deleteCapacity(const string& name) {
// //         cout << "Controller: Deleting Capacitor: " << name << endl;
// //     }
// //     void makingInductor(const string& name, const string& node1, const string& node2, const string& value_str_raw) {
// //         cout << "Controller: Making Inductor: " << name << ", " << node1 << ", " << node2 << ", Value: " << value_str_raw << endl;
// //     }
// //     void deleteInductor(const string& name) {
// //         cout << "Controller: Deleting Inductor: " << name << endl;
// //     }
// //     void makingDiode(const string& name, const string& node1, const string& node2, const string& model) {
// //         cout << "Controller: Making Diode: " << name << ", " << node1 << ", " << node2 << ", Model: " << model << endl;
// //     }
// //     void deletingDiode(const string& name) {
// //         cout << "Controller: Deleting Diode: " << name << endl;
// //     }
// //     void addingGround(const string& node) {
// //         cout << "Controller: Adding Ground to node: " << node << endl;
// //     }
// //     void deletingGround(const string& node) {
// //         cout << "Controller: Deleting Ground from node: " << node << endl;
// //     }
// //
// //     // --- Independent Sources ---
// //     void makingVoltageSourceDC(const string& name, const string& node_plus, const string& node_minus, const string& value_str_raw) {
// //         cout << "Controller: Making DC VoltageSource: " << name << ", N+: " << node_plus << ", N-: " << node_minus << ", Value: " << value_str_raw << endl;
// //     }
// //     void makingCurrentSourceDC(const string& name, const string& node_plus, const string& node_minus, const string& value_str_raw) {
// //         cout << "Controller: Making DC CurrentSource: " << name << ", N+: " << node_plus << ", N-: " << node_minus << ", Value: " << value_str_raw << endl;
// //     }
// //     void makingVoltageSourceSin(const string& name, const string& node_plus, const string& node_minus, const string& v_offset, const string& v_amplitude, const string& freq) {
// //         cout << "Controller: Making Sinusoidal Voltage Source (V): " << name << ", N+: " << node_plus << ", N-: " << node_minus
// //              << ", Voffset: " << v_offset << ", Vamplitude: " << v_amplitude << ", Freq: " << freq << endl;
// //     }
// //     void makingVoltageSourcePulse(const string& name, const string& node_plus, const string& node_minus,
// //                                   const string& v1, const string& v2, const string& td,
// //                                   const string& tr, const string& tf, const string& pw,
// //                                   const string& period) {
// //         cout << "Controller: Making PULSE Voltage Source (V): " << name << ", N+: " << node_plus << ", N-: " << node_minus
// //              << ", V1: " << v1 << ", V2: " << v2 << ", Td: " << td << ", Tr: " << tr
// //              << ", Tf: " << tf << ", PW: " << pw << ", Period: " << period << endl;
// //     }
// //
// //     // --- Dependent Sources ---
// //     void makingVCVS(const string& name, const string& n_plus, const string& n_minus, const string& nc_plus, const string& nc_minus, const string& gain_str) {
// //         cout << "Controller: Making VCVS (E): " << name << " " << n_plus << " " << n_minus << " " << nc_plus << " " << nc_minus << " Gain: " << gain_str << endl;
// //     }
// //     void makingVCCS(const string& name, const string& n_plus, const string& n_minus, const string& nc_plus, const string& nc_minus, const string& gain_str) {
// //         cout << "Controller: Making VCCS (G): " << name << " " << n_plus << " " << n_minus << " " << nc_plus << " " << nc_minus << " Gain: " << gain_str << endl;
// //     }
// //     void makingCCVS(const string& name, const string& n_plus, const string& n_minus, const string& v_control_name, const string& gain_str) {
// //         cout << "Controller: Making CCVS (H): " << name << " " << n_plus << " " << n_minus << " Vctrl: " << v_control_name << " Gain: " << gain_str << endl;
// //     }
// //     void makingCCCS(const string& name, const string& n_plus, const string& n_minus, const string& v_control_name, const string& gain_str) {
// //         cout << "Controller: Making CCCS (F): " << name << " " << n_plus << " " << n_minus << " Vctrl: " << v_control_name << " Gain: " << gain_str << endl;
// //     }
// //     void deletingSource(const string& name) {
// //         cout << "Controller: Deleting Source (any type): " << name << endl;
// //     }
// //
// //     // --- Utility ---
// //     void handleNodesCommand() {
// //         cout << "Controller: Handling 'nodes' command." << endl;
// //         cout << "Available nodes:\nn001, n002, VDD, GND, Vout, Vin" << endl;
// //     }
// //     void handleListCommand(const string& componentType = "") {
// //         if (componentType.empty()) {
// //             cout << "Controller: Handling 'list' command (all components)." << endl;
// //         } else {
// //             cout << "Controller: Handling 'list " << componentType << "' command." << endl;
// //         }
// //     }
// //     void handleRenameNodeCommand(const string& oldName, const string& newName) {
// //         cout << "Controller: Attempting to rename node " << oldName << " to " << newName << "." << endl;
// //         cout << "SUCCESS: Node renamed from " << oldName << " to " << newName << endl;
// //     }
// //     void handlePrintCommand(const string& analysisType, const string& printArgs) {
// //         cout << "Controller: Handling 'print " << analysisType << "' command with args: '" << printArgs << "'." << endl;
// //     }
// //     void handleNewFileCommand(const string& filePath) {
// //         cout << "Controller: Handling 'NewFile " << filePath << "' command." << endl;
// //     }
// //
// //     bool is_expecting_schematic_choice = false;
// //     void handleShowExistingSchematicsCommand() {
// //         cout << "Controller: Handling 'show existing schematics' command." << endl;
// //         cout << "-choose existing schematic:\n1-draft1\n2-draft2\n3-draft3\n4-elecphase1" << endl;
// //         is_expecting_schematic_choice = true;
// //     }
// //     void handleChooseSchematic(const string& choice) {
// //         cout << "Controller: Handling schematic choice: " << choice << endl;
// //         if (choice == "return") {
// //             is_expecting_schematic_choice = false;
// //             cout << "Controller: Returning to main menu..." << endl;
// //             return;
// //         }
// //         bool isValidNumericChoice = true;
// //         int chosen_num = 0;
// //         if (choice.empty() || !all_of(choice.begin(), choice.end(), ::isdigit)) {
// //             isValidNumericChoice = false;
// //         } else {
// //             try { chosen_num = stoi(choice); } catch (const std::out_of_range&) { isValidNumericChoice = false; }
// //         }
// //         if (isValidNumericChoice && chosen_num >= 1 && chosen_num <= 4) {
// //             is_expecting_schematic_choice = false;
// //             cout << "Controller: Loading schematic " << choice << "..." << endl;
// //             cout << "draft" << choice << ":\nV1 in 0 DC O AC 1 SIN(0 1 1k)\n...\n.end" << endl;
// //             handleShowExistingSchematicsCommand();
// //         } else {
// //             cout << "-Error: Inappropriate input" << endl;
// //         }
// //     }
// // };
// //
// // // Helper functions (unchanged from previous version, ensure they are correct)
// // bool checkDouble(const string& s_in) {
// //     if (s_in.empty()) return false;
// //     string s = s_in;
// //     size_t i = 0;
// //     if (s[0] == '-') {
// //         if (s.length() == 1) return false;
// //         i = 1;
// //     }
// //     bool digitFound = false;
// //     bool dotFound = false;
// //     for (; i < s.length(); ++i) {
// //         if (isdigit(s[i])) {
// //             digitFound = true;
// //         } else if (s[i] == '.') {
// //             if (dotFound) return false;
// //             dotFound = true;
// //         } else {
// //             return false;
// //         }
// //     }
// //     if (!digitFound) return false;
// //     try { stod(s); } catch (const std::invalid_argument&) { return false; } catch (const std::out_of_range&) { return false; }
// //     return true;
// // }
// //
// // bool checkingNemadElmi(const string& s_in) {
// //     if (s_in.empty()) return false;
// //     string s = s_in;
// //     std::transform(s.begin(), s.end(), s.begin(), ::tolower);
// //     size_t e_pos = s.find('e');
// //     if (e_pos == string::npos || e_pos == 0 || e_pos == s.length() - 1) return false;
// //     string base_str = s.substr(0, e_pos);
// //     string exp_str = s.substr(e_pos + 1);
// //     if (base_str.empty() || exp_str.empty()) return false;
// //     if (!checkDouble(base_str)) return false;
// //     size_t i = 0;
// //     if (exp_str[0] == '+' || exp_str[0] == '-') {
// //         if (exp_str.length() == 1) return false;
// //         i = 1;
// //     }
// //     if (i == exp_str.length()) return false;
// //     for (; i < exp_str.length(); ++i) { if (!isdigit(exp_str[i])) return false; }
// //     try { stod(s_in); } catch (const std::out_of_range&) { return false; }
// //     return true;
// // }
// //
// // pair<string, string> valuate(const string& s_in) {
// //     string s = s_in; string value_str = s; string prefix_str = "";
// //     if (s.empty()) return {s, ""};
// //     if (s.length() >= 3 && (s.substr(s.length() - 3) == "Meg" || s.substr(s.length() - 3) == "MEG")) {
// //         value_str = s.substr(0, s.length() - 3); prefix_str = "Meg";
// //     } else if (s.length() >= 2) {
// //         char last_char = s.back(); string potential_val_str = s.substr(0, s.length() - 1);
// //         if (!potential_val_str.empty() && (isdigit(potential_val_str.back()) || potential_val_str.back() == '.' || (potential_val_str.length() == 1 && potential_val_str[0] == '-'))) {
// //             switch (last_char) {
// //                 case 'G': case 'g': prefix_str = "G"; value_str = potential_val_str; break;
// //                 case 'k': case 'K': prefix_str = "k"; value_str = potential_val_str; break;
// //                 case 'm': prefix_str = "m"; value_str = potential_val_str; break;
// //                 case 'u': case 'U': prefix_str = "u"; value_str = potential_val_str; break;
// //                 case 'n': case 'N': prefix_str = "n"; value_str = potential_val_str; break;
// //                 case 'p': case 'P': prefix_str = "p"; value_str = potential_val_str; break;
// //                 case 'f': case 'F': prefix_str = "f"; value_str = potential_val_str; break;
// //             }
// //         }
// //     }
// //     if (value_str.empty() && !prefix_str.empty()) { return {s_in, ""}; }
// //     return {value_str, prefix_str};
// // }
// //
// // class CommandParser {
// // private:
// //     centralController& controller;
// //
// //     // اعتبارسنجی پارامترهای زمانی برای پالس و سینوسی
// //     // مقادیر زمانی باید غیرمنفی باشند
// //     void validateTimeParameter(const string& param_name, const string& param_val_raw, const string& src_name) {
// //         pair<string, string> p_param = valuate(param_val_raw);
// //         if (!checkDouble(p_param.first) && !checkingNemadElmi(p_param.first)) {
// //             throw logic_error("Error: Invalid format for " + param_name + " in PULSE source " + src_name + " (" + param_val_raw + ")");
// //         }
// //         if (stod(p_param.first) < 0) {
// //             throw logic_error("Error: Time parameter " + param_name + " cannot be negative in PULSE source " + src_name + " (" + param_val_raw + ")");
// //         }
// //     }
// //     // اعتبارسنجی پارامترهای ولتاژ برای پالس و سینوسی
// //     void validateVoltageParameter(const string& param_name, const string& param_val_raw, const string& src_name) {
// //          pair<string, string> p_param = valuate(param_val_raw);
// //         if (!checkDouble(p_param.first) && !checkingNemadElmi(p_param.first)) {
// //             throw logic_error("Error: Invalid format for " + param_name + " in source " + src_name + " (" + param_val_raw + ")");
// //         }
// //         // ولتاژ می تواند منفی باشد، بنابراین بررسی دیگری لازم نیست
// //     }
// //
// //
// //     bool tryParseElementCommands(const string& in) {
// //         smatch matches;
// //         regex add_res_pattern(R"(^add (R\w*) (\w+) (\w+) (.+?)$)");
// //         regex del_res_pattern(R"(^delete (R\w+)$)");
// //         if (regex_match(in, matches, add_res_pattern)) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
// //             pair<string, string> p_val = valuate(val_raw);
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for " + name);
// //             double num_val = stod(p_val.first);
// //             if (num_val <= 0) throw logic_error("Error: Resistance cannot be zero or negative");
// //             controller.makingResistor(name, n1, n2, val_raw); return true;
// //         }
// //         if (regex_match(in, matches, del_res_pattern)) { controller.deletingResistor(matches[1].str()); return true; }
// //
// //         regex add_cap_pattern(R"(^add (C\w+) (\w+) (\w+) (.+?)$)");
// //         regex del_cap_pattern(R"(^delete (C\w+)$)");
// //         if (regex_match(in, matches, add_cap_pattern)) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
// //             pair<string, string> p_val = valuate(val_raw);
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for " + name);
// //             double num_val = stod(p_val.first);
// //             if (num_val <= 0) throw logic_error("Error: Capacitance cannot be zero or negative");
// //             controller.makingCapacity(name, n1, n2, val_raw); return true;
// //         }
// //         if (regex_match(in, matches, del_cap_pattern)) { controller.deleteCapacity(matches[1].str()); return true; }
// //
// //         regex add_ind_pattern(R"(^add (L\w+) (\w+) (\w+) (.+?)$)");
// //         regex del_ind_pattern(R"(^delete (L\w+)$)");
// //         if (regex_match(in, matches, add_ind_pattern)) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
// //             pair<string, string> p_val = valuate(val_raw);
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for " + name);
// //             double num_val = stod(p_val.first);
// //             if (num_val <= 0) throw logic_error("Error: Inductance cannot be zero or negative");
// //             controller.makingInductor(name, n1, n2, val_raw); return true;
// //         }
// //         if (regex_match(in, matches, del_ind_pattern)) { controller.deleteInductor(matches[1].str()); return true; }
// //
// //         regex add_diode_pattern(R"(^add (D\w+) (\w+) (\w+) (D|Z)$)");
// //         regex del_diode_pattern(R"(^delete (D\w+)$)");
// //         if (regex_match(in, matches, add_diode_pattern)) { controller.makingDiode(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true; }
// //         if (regex_match(in, matches, del_diode_pattern)) { controller.deletingDiode(matches[1].str()); return true; }
// //
// //         regex add_gnd_pattern(R"(^add GND (\w+)$)");
// //         regex del_gnd_pattern(R"(^delete GND (\w+)$)");
// //         if (regex_match(in, matches, add_gnd_pattern)) { controller.addingGround(matches[1].str()); return true; }
// //         if (regex_match(in, matches, del_gnd_pattern)) { controller.deletingGround(matches[1].str()); return true; }
// //         return false;
// //     }
// //
// //     bool tryParseSourceCommands(const string& in) {
// //         smatch matches;
// //
// //         // Independent DC Voltage Source: add VoltageSource<Name> <Node1> <Node2> <Value>
// //         regex add_vdc_keyword_pattern(R"(^add VoltageSource(\w+) (\w+) (\w+) (.+?)$)");
// //         if (regex_match(in, matches, add_vdc_keyword_pattern)) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
// //             pair<string, string> p_val = valuate(val_raw);
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for DC VoltageSource " + name);
// //             controller.makingVoltageSourceDC(name, n1, n2, val_raw); return true;
// //         }
// //
// //         // Independent DC Current Source: add CurrentSource<Name> <Node1> <Node2> <Value>
// //         regex add_idc_keyword_pattern(R"(^add CurrentSource(\w+) (\w+) (\w+) (.+?)$)");
// //         if (regex_match(in, matches, add_idc_keyword_pattern)) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
// //             pair<string, string> p_val = valuate(val_raw);
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for DC CurrentSource " + name);
// //             controller.makingCurrentSourceDC(name, n1, n2, val_raw); return true;
// //         }
// //
// //         // Voltage Sources (V type) - SIN or PULSE or generic (interpreted as DC if not SIN/PULSE)
// //         regex add_v_generic_pattern(R"(^add (V\w+) (\w+) (\w+) (.+?)$)");
// //         if (regex_match(in, matches, add_v_generic_pattern)) {
// //             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_full_str = matches[4].str();
// //
// //             // Check for SIN: SIN(<Voffset>,<Vamplitude>,<Frequency>)
// //             regex vsin_params_pattern(R"(^SIN\s*\(\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*\)$)");
// //             smatch sin_matches;
// //             if (regex_match(val_full_str, sin_matches, vsin_params_pattern)) {
// //                 string voff_raw=sin_matches[1].str(), vamp_raw=sin_matches[2].str(), freq_raw=sin_matches[3].str();
// //                 validateVoltageParameter("Voffset", voff_raw, name);
// //                 validateVoltageParameter("Vamplitude", vamp_raw, name);
// //                 validateTimeParameter("Frequency", freq_raw, name); // Frequency is a time-related param (1/T)
// //                 if (stod(valuate(freq_raw).first) <= 0) throw logic_error("Error: Frequency for SIN source " + name + " must be positive.");
// //                 controller.makingVoltageSourceSin(name, n1, n2, voff_raw, vamp_raw, freq_raw); return true;
// //             }
// //
// //             // Check for PULSE: PULSE(<V1> <V2> <Td> <Tr> <Tf> <PW> <Period>)
// //             // Regex to capture 7 parameters for PULSE, allowing spaces and scientific notation
// //             regex vpulse_params_pattern(R"(^PULSE\s*\(\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*\)$)");
// //             smatch pulse_matches;
// //             if (regex_match(val_full_str, pulse_matches, vpulse_params_pattern)) {
// //                 if (pulse_matches.size() == 8) { // 1 overall match + 7 parameters
// //                     string p_v1 = pulse_matches[1].str(); string p_v2 = pulse_matches[2].str();
// //                     string p_td = pulse_matches[3].str(); string p_tr = pulse_matches[4].str();
// //                     string p_tf = pulse_matches[5].str(); string p_pw = pulse_matches[6].str();
// //                     string p_period = pulse_matches[7].str();
// //
// //                     validateVoltageParameter("V1", p_v1, name); validateVoltageParameter("V2", p_v2, name);
// //                     validateTimeParameter("Td", p_td, name);     validateTimeParameter("Tr", p_tr, name);
// //                     validateTimeParameter("Tf", p_tf, name);     validateTimeParameter("PW", p_pw, name);
// //                     validateTimeParameter("Period", p_period, name);
// //                     // Additional specific checks for PULSE might be needed (e.g. Tr, Tf, PW, Period >= 0)
// //                     // The validateTimeParameter already checks for >=0.
// //                     controller.makingVoltageSourcePulse(name, n1, n2, p_v1, p_v2, p_td, p_tr, p_tf, p_pw, p_period);
// //                     return true;
// //                 } else {
// //                      throw logic_error("Error: Incorrect number of parameters for PULSE source " + name);
// //                 }
// //             }
// //
// //             // If not SIN or PULSE, and starts with V, it must be a plain DC source using the V<name> format (not VoltageSource<name>)
// //             // This path is taken if "VoltageSource<name>" was not matched earlier.
// //             // This allows for "add Vname n1 n2 value" as a DC source.
// //             pair<string, string> p_val = valuate(val_full_str);
// //             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for DC voltage source (V) " + name);
// //             controller.makingVoltageSourceDC(name, n1, n2, val_full_str); // Use a different controller method or flag if Vname syntax is distinct
// //             return true;
// //         }
// //
// //         // Dependent Sources (PDF 18.4)
// //         regex add_vcvs_pattern(R"(^add (E\w+) (\w+) (\w+) (\w+) (\w+) (.+?)$)");
// //         if (regex_match(in, matches, add_vcvs_pattern)) {
// //             string name=matches[1].str(), gain_raw=matches[6].str();
// //             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain format for VCVS " + name);
// //             controller.makingVCVS(name, matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str(), gain_raw); return true;
// //         }
// //         regex add_vccs_pattern(R"(^add (G\w+) (\w+) (\w+) (\w+) (\w+) (.+?)$)");
// //         if (regex_match(in, matches, add_vccs_pattern)) {
// //             string name=matches[1].str(), gain_raw=matches[6].str();
// //             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain format for VCCS " + name);
// //             controller.makingVCCS(name, matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str(), gain_raw); return true;
// //         }
// //         regex add_ccvs_pattern(R"(^add (H\w+) (\w+) (\w+) (V\w+) (.+?)$)"); // Controlling source name must start with V
// //         if (regex_match(in, matches, add_ccvs_pattern)) {
// //             string name=matches[1].str(), gain_raw=matches[5].str();
// //             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain format for CCVS " + name);
// //             controller.makingCCVS(name, matches[2].str(), matches[3].str(), matches[4].str(), gain_raw); return true;
// //         }
// //         regex add_cccs_pattern(R"(^add (F\w+) (\w+) (\w+) (V\w+) (.+?)$)"); // Controlling source name must start with V
// //         if (regex_match(in, matches, add_cccs_pattern)) {
// //             string name=matches[1].str(), gain_raw=matches[5].str();
// //             if(!checkDouble(valuate(gain_raw).first) && !checkingNemadElmi(valuate(gain_raw).first)) throw logic_error("Error: Invalid Gain format for CCCS " + name);
// //             controller.makingCCCS(name, matches[2].str(), matches[3].str(), matches[4].str(), gain_raw); return true;
// //         }
// //
// //         // Generic delete for any source type whose name was defined starting with V, I, E, G, H, F
// //         // This also needs to handle VoltageSource<Name> and CurrentSource<Name>
// //         // For VoltageSource<Name>, name is <Name>, so it won't match [VIEGHF]\w+
// //         // Need a more specific delete or the controller handles deletion based on full name.
// //         // For now, this deletes Vname, Iname, Ename etc.
// //         regex del_src_name_pattern(R"(^delete (\w+)$)"); // Generic delete by name
// //         if (regex_match(in, matches, del_src_name_pattern)) {
// //             string name_to_delete = matches[1].str();
// //             // Controller needs to determine if 'name_to_delete' is a source of any kind (V,I,E,G,H,F, VoltageSourceX, CurrentSourceY)
// //             // This simple regex might be too broad if element names can be similar.
// //             // For now, assume controller checks if it's a known source name.
// //             // The PDF for deleting elements uses `delete R<name>`, `delete C<name>`, etc.
// //             // It does not specify a delete format for sources. Let's assume `delete <SourceName>` is generic.
// //             controller.deletingSource(name_to_delete);
// //             return true;
// //         }
// //         return false;
// //     }
// //
// //     bool tryParseNodeCommands(const string& in) {
// //         smatch matches;
// //         regex nodes_pattern(R"(^nodes$)");
// //         if (regex_match(in, matches, nodes_pattern)) { controller.handleNodesCommand(); return true; }
// //         regex rename_node_base_pattern(R"(^rename node)");
// //         regex rename_node_full_pattern(R"(^rename node (\w+) (\w+)$)");
// //         if (regex_match(in, matches, rename_node_full_pattern)) { controller.handleRenameNodeCommand(matches[1].str(), matches[2].str()); return true;
// //         } else if (regex_search(in, rename_node_base_pattern)) {
// //             throw logic_error("ERROR: Invalid syntax correct format: rename node <old_name> <new_name>");
// //         }
// //         return false;
// //     }
// //
// //     bool tryParseListCommands(const string& in) {
// //         smatch matches;
// //         regex list_all_pattern(R"(^list$)");
// //         if (regex_match(in, matches, list_all_pattern)) { controller.handleListCommand(); return true; }
// //         regex list_comp_pattern(R"(^list ([RCLDVIEGHF])$)");
// //         if (regex_match(in, matches, list_comp_pattern)) { controller.handleListCommand(matches[1].str()); return true; }
// //         return false;
// //     }
// //
// //     bool tryParsePrintCommands(const string& in) {
// //         smatch matches;
// //         regex print_pattern(R"(^print (TRAN|DC|AC) (.+)$)");
// //         if (regex_match(in, matches, print_pattern)) {
// //             try { controller.handlePrintCommand(matches[1].str(), matches[2].str());
// //             } catch (const logic_error& e) {
// //                 string err_msg = e.what();
// //                 if (err_msg.find("not found in circuit") != string::npos ) { throw; }
// //                 throw logic_error("Syntax error in command");
// //             }
// //             return true;
// //         }
// //         return false;
// //     }
// //
// //     bool tryParseFileAndMenuCommands(const string& in) {
// //         smatch matches;
// //         regex new_file_pattern(R"(^NewFile\s+(.+)$)");
// //         if (regex_match(in, matches, new_file_pattern)) { controller.handleNewFileCommand(matches[1].str()); return true;}
// //         regex show_schematics_pattern(R"(^show existing schematics$)");
// //         if (regex_match(in, matches, show_schematics_pattern)) { controller.handleShowExistingSchematicsCommand(); return true;}
// //         if (controller.is_expecting_schematic_choice) { controller.handleChooseSchematic(in); return true; }
// //         return false;
// //     }
// //
// // public:
// //     explicit CommandParser(centralController& ctrl) : controller(ctrl) {}
// //
// //     void processInput(const string& in) {
// //         if (in.empty()) { return; }
// //         if (tryParseFileAndMenuCommands(in)) return;
// //         if (tryParseNodeCommands(in)) return;
// //         if (tryParseListCommands(in)) return;
// //         if (tryParsePrintCommands(in)) return;
// //         if (tryParseElementCommands(in)) return;
// //         if (tryParseSourceCommands(in)) return;
// //         throw logic_error("Error: Unknown command or invalid syntax. -> " + in);
// //     }
// // };
// //
// // int main() {
// //     centralController controller_instance;
// //     CommandParser parser(controller_instance);
// //
// //     vector<string> test_commands = {
// //         // --- Section 13 Elements ---
// //         "add R1 N1 N2 1k",
// //         "add L1 N1 N_intermediate 1m",
// //         "add D1 N_intermediate N2 D",
// //         "add GND 0",
// //         "delete R1",
// //         // --- Section 14 Utility ---
// //         "nodes", "list", "list R", "rename node N001 Vout", "rename node N002",
// //         // --- Section 15 Print ---
// //         "print TRAN V(n001) I(R1)",
// //         // --- Section 17 File ---
// //         "NewFile /path/to/circuit.txt",
// //         // --- Section 18 Independent Sources ---
// //         // DC Sources with keywords
// //         "add VoltageSourceVS1 N_plus N_minus 5",
// //         "add VoltageSourceV_bat N1 0 -9.0",
// //         "add CurrentSourceIS1 N_from N_to 0.5m",
// //         "add CurrentSourceI_load N_sense GND 10uA", // Test with unit in value (valuate might strip A)
// //         // SIN Source (V<name> syntax)
// //         "add Vac N_in 0 SIN(0,5,1k)",
// //         "add Vsig P1 P2 SIN( 1.0, 2.5m, 500 )",
// //         // PULSE Source (V<name> syntax)
// //         "add Vclk clk 0 PULSE(0 1 1n 0.1n 0.1n 5n 10n)",       // Valid pulse
// //         "add Vdata dat 0 PULSE(0, 3.3, 0, 10p, 10p, 1u, 2u)", // Valid with spaces
// //         "add Vpulse_err1 N1 N2 PULSE(0 1 1n 0.1n 0.1n 5n)",      // Too few params for PULSE
// //         "add Vpulse_err2 N1 N2 PULSE(0 1 1n 0.1n -0.1n 5n 10n)",// Negative Tf
// //         "add Vpulse_err3 N1 N2 PULSE(0, one, 1n, 0.1n, 0.1n, 5n, 10n)", // Invalid V2
// //         // --- Section 18 Dependent Sources ---
// //         "add Eamp Nout Nin Nctrl1 Nctrl2 100",
// //         "add Ggm N_drain N_source N_gate N_source 0.01",
// //         "add Hcurrsens N_vout N_gnd Vmeas_src 50", // Vmeas_src must be a V type source
// //         "add Fcurrmirror N_out_coll N_gnd Vbias_src 1.0", // Vbias_src must be a V type source
// //         "add E_bad_gain N1 N2 N3 N4 10kOhm", // Invalid gain format
// //         "add H_bad_vctrl N1 N2 Rcontrol 10", // Invalid control source name (not V\w*)
// //         // --- Deleting Sources ---
// //         "delete VS1", // Assumes VS1 was defined via VoltageSourceVS1
// //         "delete Vac",
// //         "delete Eamp",
// //         // --- Error cases from previous tests ---
// //         "add RX N1 N2 0",
// //         "add R2 N3 N4 -100",
// //         "add R5 N9 N10 1.2e",
// //         "add Rtest N1 N2 value_without_prefix",
// //         "add L_bad_unit N1 N2 1mH",
// //         "unknown command here",
// //         "add",
// //         "delete"
// //     };
// //
// //     cout << "--- Starting Command Tests ---" << endl;
// //     for (const string& cmd : test_commands) {
// //         cout << "\nInput: \"" << cmd << "\"" << endl;
// //         try {
// //             parser.processInput(cmd);
// //         } catch (const logic_error& e) {
// //             cout << "Exception: " << e.what() << endl;
// //         }
// //     }
// //     cout << "\n--- Testing Schematic Choice Menu ---" << endl;
// //     string menu_inputs[] = {"show existing schematics", "5", "draft2", "1", "return"};
// //     for (const string& cmd : menu_inputs) {
// //         cout << "\nInput: \"" << cmd << "\"" << endl;
// //         try {
// //             parser.processInput(cmd);
// //         } catch (const logic_error& e) {
// //             cout << "Exception: " << e.what() << endl;
// //         }
// //     }
// //     return 0;
// // }
//
//
// #include <iostream>
// #include <string>
// #include <vector>
// #include <regex>
// #include <stdexcept> // For logic_error
// #include <algorithm> // For tolower, transform
// #include <iomanip>
//
// using namespace std;
//
// // Forward declaration of centralController
// class centralController {
// public:
//     // --- Elements ---
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
//     // --- Independent Sources ---
//     void makingVoltageSourceDC(const string& name, const string& node_plus, const string& node_minus, const string& value_str_raw) {
//         cout << "Controller: Making DC VoltageSource: " << name << ", N+: " << node_plus << ", N-: " << node_minus << ", Value: " << value_str_raw << endl;
//     }
//     void makingCurrentSourceDC(const string& name, const string& node_plus, const string& node_minus, const string& value_str_raw) {
//         cout << "Controller: Making DC CurrentSource: " << name << ", N+: " << node_plus << ", N-: " << node_minus << ", Value: " << value_str_raw << endl;
//     }
//     void makingVoltageSourceSin(const string& name, const string& node_plus, const string& node_minus, const string& v_offset, const string& v_amplitude, const string& freq) {
//         cout << "Controller: Making Sinusoidal Voltage Source (V): " << name << ", N+: " << node_plus << ", N-: " << node_minus
//              << ", Voffset: " << v_offset << ", Vamplitude: " << v_amplitude << ", Freq: " << freq << endl;
//     }
//     void makingVoltageSourcePulse(const string& name, const string& node_plus, const string& node_minus,
//                                   const string& v1, const string& v2, const string& td,
//                                   const string& tr, const string& tf, const string& pw,
//                                   const string& period) {
//         cout << "Controller: Making PULSE Voltage Source (V): " << name << ", N+: " << node_plus << ", N-: " << node_minus
//              << ", V1: " << v1 << ", V2: " << v2 << ", Td: " << td << ", Tr: " << tr
//              << ", Tf: " << tf << ", PW: " << pw << ", Period: " << period << endl;
//     }
//
//     // --- Dependent Sources ---
//     void makingVCVS(const string& name, const string& n_plus, const string& n_minus, const string& nc_plus, const string& nc_minus, const string& gain_str) {
//         cout << "Controller: Making VCVS (E): " << name << " " << n_plus << " " << n_minus << " " << nc_plus << " " << nc_minus << " Gain: " << gain_str << endl;
//     }
//     void makingVCCS(const string& name, const string& n_plus, const string& n_minus, const string& nc_plus, const string& nc_minus, const string& gain_str) {
//         cout << "Controller: Making VCCS (G): " << name << " " << n_plus << " " << n_minus << " " << nc_plus << " " << nc_minus << " Gain: " << gain_str << endl;
//     }
//     void makingCCVS(const string& name, const string& n_plus, const string& n_minus, const string& v_control_name, const string& gain_str) {
//         cout << "Controller: Making CCVS (H): " << name << " " << n_plus << " " << n_minus << " Vctrl: " << v_control_name << " Gain: " << gain_str << endl;
//     }
//     void makingCCCS(const string& name, const string& n_plus, const string& n_minus, const string& v_control_name, const string& gain_str) {
//         cout << "Controller: Making CCCS (F): " << name << " " << n_plus << " " << n_minus << " Vctrl: " << v_control_name << " Gain: " << gain_str << endl;
//     }
//     void deletingSource(const string& name) {
//         cout << "Controller: Deleting Source (any type): " << name << endl;
//     }
//
//     // --- Analysis Definition (Section 20) ---
//     void defineTransientAnalysis(const string& tstep, const string& tstop, const string& tstart, const string& tmaxstep) {
//         cout << "Controller: Defining Transient Analysis: Tstep=" << tstep << ", Tstop=" << tstop;
//         if (!tstart.empty()) cout << ", Tstart=" << tstart;
//         if (!tmaxstep.empty()) cout << ", Tmaxstep=" << tmaxstep;
//         cout << endl;
//     }
//     void defineDCAnalysis(const string& srcName, const string& startVal, const string& endVal, const string& increment) {
//         cout << "Controller: Defining DC Analysis: Source=" << srcName << ", Start=" << startVal
//              << ", End=" << endVal << ", Increment=" << increment << endl;
//     }
//
//     // --- Utility & Print (Section 14, 15) ---
//     void handleNodesCommand() {
//         cout << "Controller: Handling 'nodes' command." << endl;
//         cout << "Available nodes:\nn001, n002, VDD, GND, Vout, Vin" << endl;
//     }
//     void handleListCommand(const string& componentType = "") {
//         if (componentType.empty()) {
//             cout << "Controller: Handling 'list' command (all components)." << endl;
//         } else {
//             cout << "Controller: Handling 'list " << componentType << "' command." << endl;
//         }
//     }
//     void handleRenameNodeCommand(const string& oldName, const string& newName) {
//         cout << "Controller: Attempting to rename node " << oldName << " to " << newName << "." << endl;
//         cout << "SUCCESS: Node renamed from " << oldName << " to " << newName << endl;
//     }
//     // Modified to accept analysis parameters separately
//     void handlePrintCommand(const string& analysisType,
//                             const vector<string>& analysisParams,
//                             const string& outputVarsStr) {
//         cout << "Controller: Handling 'print " << analysisType << "' command." << endl;
//         if (!analysisParams.empty()) {
//             cout << "  With analysis params: ";
//             for (const auto& p : analysisParams) cout << p << " ";
//             cout << endl;
//         }
//         cout << "  Output Vars: '" << outputVarsStr << "'." << endl;
//     }
//     void handleNewFileCommand(const string& filePath) {
//         cout << "Controller: Handling 'NewFile " << filePath << "' command." << endl;
//     }
//
//     bool is_expecting_schematic_choice = false;
//     void handleShowExistingSchematicsCommand() {
//         cout << "Controller: Handling 'show existing schematics' command." << endl;
//         cout << "-choose existing schematic:\n1-draft1\n2-draft2\n3-draft3\n4-elecphase1" << endl;
//         is_expecting_schematic_choice = true;
//     }
//     void handleChooseSchematic(const string& choice) {
//         cout << "Controller: Handling schematic choice: " << choice << endl;
//         if (choice == "return") {
//             is_expecting_schematic_choice = false;
//             cout << "Controller: Returning to main menu..." << endl;
//             return;
//         }
//         bool isValidNumericChoice = true;
//         int chosen_num = 0;
//         if (choice.empty() || !all_of(choice.begin(), choice.end(), ::isdigit)) {
//             isValidNumericChoice = false;
//         } else {
//             try { chosen_num = stoi(choice); } catch (const std::out_of_range&) { isValidNumericChoice = false; }
//         }
//         if (isValidNumericChoice && chosen_num >= 1 && chosen_num <= 4) {
//             is_expecting_schematic_choice = false;
//             cout << "Controller: Loading schematic " << choice << "..." << endl;
//             cout << "draft" << choice << ":\nV1 in 0 DC O AC 1 SIN(0 1 1k)\n...\n.end" << endl;
//             handleShowExistingSchematicsCommand();
//         } else {
//             cout << "-Error: Inappropriate input" << endl;
//         }
//     }
// };
//
// // Helper functions
// bool checkDouble(const string& s_in) {
//     if (s_in.empty()) return false; string s = s_in; size_t i = 0;
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
// bool checkingNemadElmi(const string& s_in) {
//     if (s_in.empty()) return false; string s = s_in; std::transform(s.begin(), s.end(), s.begin(), ::tolower);
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
// // Helper function to trim leading/trailing whitespace
// string trim(const string& str) {
//     const string whitespace = " \t\n\r\f\v";
//     size_t start = str.find_first_not_of(whitespace);
//     if (string::npos == start) return ""; // no content
//     size_t end = str.find_last_not_of(whitespace);
//     return str.substr(start, end - start + 1);
// }
//
//
// class CommandParser {
// private:
//     centralController& controller;
//
//     void validateTimeParameter(const string& param_name, const string& param_val_raw, const string& context_name, bool allow_zero = true) {
//         pair<string, string> p_param = valuate(param_val_raw);
//         if (!checkDouble(p_param.first) && !checkingNemadElmi(p_param.first)) {
//             throw logic_error("Error: Invalid format for " + param_name + " in " + context_name + " (" + param_val_raw + ")");
//         }
//         double val = stod(p_param.first);
//         if (val < 0) {
//             throw logic_error("Error: Time parameter " + param_name + " cannot be negative in " + context_name + " (" + param_val_raw + ")");
//         }
//         if (!allow_zero && val == 0) {
//              throw logic_error("Error: Time parameter " + param_name + " cannot be zero in " + context_name + " (" + param_val_raw + ")");
//         }
//     }
//     void validateVoltageParameter(const string& param_name, const string& param_val_raw, const string& context_name) {
//          pair<string, string> p_param = valuate(param_val_raw);
//         if (!checkDouble(p_param.first) && !checkingNemadElmi(p_param.first)) {
//             throw logic_error("Error: Invalid format for " + param_name + " in " + context_name + " (" + param_val_raw + ")");
//         }
//     }
//     void validateNumericParameter(const string& param_name, const string& param_val_raw, const string& context_name, bool allow_zero = true) {
//         pair<string, string> p_param = valuate(param_val_raw);
//         if (!checkDouble(p_param.first) && !checkingNemadElmi(p_param.first)) {
//             throw logic_error("Error: Invalid numeric format for " + param_name + " in " + context_name + " (" + param_val_raw + ")");
//         }
//         if (!allow_zero && stod(p_param.first) == 0) {
//             throw logic_error("Error: Parameter " + param_name + " cannot be zero in " + context_name + " (" + param_val_raw + ")");
//         }
//     }
//
//
//     bool tryParseElementCommands(const string& in) {
//         smatch matches;
//         regex add_res_pattern(R"(^add (R\w*) (\w+) (\w+) (.+?)$)");
//         regex del_res_pattern(R"(^delete (R\w+)$)");
//         if (regex_match(in, matches, add_res_pattern)) {
//             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
//             pair<string, string> p_val = valuate(val_raw);
//             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for " + name);
//             double num_val = stod(p_val.first);
//             if (num_val <= 0) throw logic_error("Error: Resistance cannot be zero or negative");
//             controller.makingResistor(name, n1, n2, val_raw); return true;
//         }
//         if (regex_match(in, matches, del_res_pattern)) { controller.deletingResistor(matches[1].str()); return true; }
//
//         regex add_cap_pattern(R"(^add (C\w+) (\w+) (\w+) (.+?)$)");
//         regex del_cap_pattern(R"(^delete (C\w+)$)");
//         if (regex_match(in, matches, add_cap_pattern)) {
//             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
//             pair<string, string> p_val = valuate(val_raw);
//             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for " + name);
//             double num_val = stod(p_val.first);
//             if (num_val <= 0) throw logic_error("Error: Capacitance cannot be zero or negative");
//             controller.makingCapacity(name, n1, n2, val_raw); return true;
//         }
//         if (regex_match(in, matches, del_cap_pattern)) { controller.deleteCapacity(matches[1].str()); return true; }
//
//         regex add_ind_pattern(R"(^add (L\w+) (\w+) (\w+) (.+?)$)");
//         regex del_ind_pattern(R"(^delete (L\w+)$)");
//         if (regex_match(in, matches, add_ind_pattern)) {
//             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
//             pair<string, string> p_val = valuate(val_raw);
//             if (!checkDouble(p_val.first) && !checkingNemadElmi(p_val.first)) throw logic_error("Error: Invalid value format for " + name);
//             double num_val = stod(p_val.first);
//             if (num_val <= 0) throw logic_error("Error: Inductance cannot be zero or negative");
//             controller.makingInductor(name, n1, n2, val_raw); return true;
//         }
//         if (regex_match(in, matches, del_ind_pattern)) { controller.deleteInductor(matches[1].str()); return true; }
//
//         regex add_diode_pattern(R"(^add (D\w+) (\w+) (\w+) (D|Z)$)");
//         regex del_diode_pattern(R"(^delete (D\w+)$)");
//         if (regex_match(in, matches, add_diode_pattern)) { controller.makingDiode(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true; }
//         if (regex_match(in, matches, del_diode_pattern)) { controller.deletingDiode(matches[1].str()); return true; }
//
//         regex add_gnd_pattern(R"(^add GND (\w+)$)");
//         regex del_gnd_pattern(R"(^delete GND (\w+)$)");
//         if (regex_match(in, matches, add_gnd_pattern)) { controller.addingGround(matches[1].str()); return true; }
//         if (regex_match(in, matches, del_gnd_pattern)) { controller.deletingGround(matches[1].str()); return true; }
//         return false;
//     }
//
//     bool tryParseSourceCommands(const string& in) {
//         smatch matches;
//         regex add_vdc_keyword_pattern(R"(^add VoltageSource(\w+) (\w+) (\w+) (.+?)$)");
//         if (regex_match(in, matches, add_vdc_keyword_pattern)) {
//             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
//             validateVoltageParameter("Value", val_raw, "VoltageSource " + name);
//             controller.makingVoltageSourceDC(name, n1, n2, val_raw); return true;
//         }
//         regex add_idc_keyword_pattern(R"(^add CurrentSource(\w+) (\w+) (\w+) (.+?)$)");
//         if (regex_match(in, matches, add_idc_keyword_pattern)) {
//             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_raw = matches[4].str();
//             validateNumericParameter("Value", val_raw, "CurrentSource " + name); // Can be any number
//             controller.makingCurrentSourceDC(name, n1, n2, val_raw); return true;
//         }
//         regex add_v_generic_pattern(R"(^add (V\w+) (\w+) (\w+) (.+?)$)");
//         if (regex_match(in, matches, add_v_generic_pattern)) {
//             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_full_str = matches[4].str();
//             regex vsin_params_pattern(R"(^SIN\s*\(\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*\)$)");
//             smatch sin_matches;
//             if (regex_match(val_full_str, sin_matches, vsin_params_pattern)) {
//                 string voff_raw=sin_matches[1].str(), vamp_raw=sin_matches[2].str(), freq_raw=sin_matches[3].str();
//                 validateVoltageParameter("Voffset", voff_raw, "SIN source " + name);
//                 validateVoltageParameter("Vamplitude", vamp_raw, "SIN source " + name);
//                 validateTimeParameter("Frequency", freq_raw, "SIN source " + name, false); // Freq > 0
//                 controller.makingVoltageSourceSin(name, n1, n2, voff_raw, vamp_raw, freq_raw); return true;
//             }
//             regex vpulse_params_pattern(R"(^PULSE\s*\(\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*\)$)");
//             smatch pulse_matches;
//             if (regex_match(val_full_str, pulse_matches, vpulse_params_pattern)) {
//                 if (pulse_matches.size() == 8) {
//                     string p_v1 = pulse_matches[1].str(); string p_v2 = pulse_matches[2].str();
//                     string p_td = pulse_matches[3].str(); string p_tr = pulse_matches[4].str();
//                     string p_tf = pulse_matches[5].str(); string p_pw = pulse_matches[6].str();
//                     string p_period = pulse_matches[7].str();
//                     validateVoltageParameter("V1", p_v1, "PULSE source " + name); validateVoltageParameter("V2", p_v2, "PULSE source " + name);
//                     validateTimeParameter("Td", p_td, "PULSE source " + name);     validateTimeParameter("Tr", p_tr, "PULSE source " + name);
//                     validateTimeParameter("Tf", p_tf, "PULSE source " + name);     validateTimeParameter("PW", p_pw, "PULSE source " + name);
//                     validateTimeParameter("Period", p_period, "PULSE source " + name, false); // Period must be > 0 for repetitive pulse
//                     controller.makingVoltageSourcePulse(name, n1, n2, p_v1, p_v2, p_td, p_tr, p_tf, p_pw, p_period);
//                     return true;
//                 } else { throw logic_error("Error: Incorrect number of parameters for PULSE source " + name); }
//             }
//             validateVoltageParameter("Value", val_full_str, "DC source V" + name);
//             controller.makingVoltageSourceDC(name, n1, n2, val_full_str);
//             return true;
//         }
//         regex add_vcvs_pattern(R"(^add (E\w+) (\w+) (\w+) (\w+) (\w+) (.+?)$)");
//         if (regex_match(in, matches, add_vcvs_pattern)) {
//             string name=matches[1].str(), gain_raw=matches[6].str();
//             validateNumericParameter("Gain", gain_raw, "VCVS " + name);
//             controller.makingVCVS(name, matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str(), gain_raw); return true;
//         }
//         regex add_vccs_pattern(R"(^add (G\w+) (\w+) (\w+) (\w+) (\w+) (.+?)$)");
//         if (regex_match(in, matches, add_vccs_pattern)) {
//             string name=matches[1].str(), gain_raw=matches[6].str();
//             validateNumericParameter("Gain", gain_raw, "VCCS " + name);
//             controller.makingVCCS(name, matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str(), gain_raw); return true;
//         }
//         regex add_ccvs_pattern(R"(^add (H\w+) (\w+) (\w+) (V\w+) (.+?)$)");
//         if (regex_match(in, matches, add_ccvs_pattern)) {
//             string name=matches[1].str(), gain_raw=matches[5].str();
//             validateNumericParameter("Gain", gain_raw, "CCVS " + name);
//             controller.makingCCVS(name, matches[2].str(), matches[3].str(), matches[4].str(), gain_raw); return true;
//         }
//         regex add_cccs_pattern(R"(^add (F\w+) (\w+) (\w+) (V\w+) (.+?)$)");
//         if (regex_match(in, matches, add_cccs_pattern)) {
//             string name=matches[1].str(), gain_raw=matches[5].str();
//             validateNumericParameter("Gain", gain_raw, "CCCS " + name);
//             controller.makingCCCS(name, matches[2].str(), matches[3].str(), matches[4].str(), gain_raw); return true;
//         }
//         regex del_src_name_pattern(R"(^delete (\w+)$)");
//         if (regex_match(in, matches, del_src_name_pattern)) {
//             string name_to_delete = matches[1].str();
//             if (name_to_delete.rfind("VoltageSource", 0) == 0 ||
//                 name_to_delete.rfind("CurrentSource", 0) == 0 ||
//                 (name_to_delete.length() > 0 && string("VIEGHF").find(name_to_delete[0]) != string::npos)) {
//                 controller.deletingSource(name_to_delete);
//                 return true;
//             }
//         }
//         return false;
//     }
//
//     // Section 20: Analysis Definition Commands
//     bool tryParseAnalysisDefinitionCommands(const string& in) {
//         smatch matches;
//         regex tran_def_pattern(R"(^\.TRAN\s+([^\s]+)\s+([^\s]+)(?:\s+([^\s]+))?(?:\s+([^\s]+))?\s*$)");
//         if (regex_match(in, matches, tran_def_pattern)) {
//             string tstep = matches[1].str();
//             string tstop = matches[2].str();
//             string tstart = matches[3].matched ? matches[3].str() : "";
//             string tmaxstep = matches[4].matched ? matches[4].str() : "";
//
//             validateTimeParameter("Tstep", tstep, ".TRAN analysis", false);
//             validateTimeParameter("Tstop", tstop, ".TRAN analysis", false);
//             if (stod(valuate(tstop).first) <= stod(valuate(tstep).first)) {
//                 throw logic_error("Error: Tstop must be greater than Tstep in .TRAN analysis.");
//             }
//             if (!tstart.empty()) validateTimeParameter("Tstart", tstart, ".TRAN analysis");
//             if (!tmaxstep.empty()) validateTimeParameter("Tmaxstep", tmaxstep, ".TRAN analysis", false);
//
//             controller.defineTransientAnalysis(tstep, tstop, tstart, tmaxstep);
//             return true;
//         }
//
//         regex dc_def_pattern(R"(^\.DC\s+([VI]\w*)\s+([^\s]+)\s+([^\s]+)\s+([^\s]+)\s*$)");
//         if (regex_match(in, matches, dc_def_pattern)) {
//             string srcName = matches[1].str();
//             string startVal = matches[2].str();
//             string endVal = matches[3].str();
//             string increment = matches[4].str();
//
//             validateNumericParameter("StartValue", startVal, ".DC analysis for " + srcName);
//             validateNumericParameter("EndValue", endVal, ".DC analysis for " + srcName);
//             validateNumericParameter("Increment", increment, ".DC analysis for " + srcName, false);
//
//             controller.defineDCAnalysis(srcName, startVal, endVal, increment);
//             return true;
//         }
//         return false;
//     }
//
//
//     bool tryParseNodeCommands(const string& in) {
//         smatch matches;
//         regex nodes_pattern(R"(^nodes$)");
//         if (regex_match(in, matches, nodes_pattern)) { controller.handleNodesCommand(); return true; }
//         regex rename_node_base_pattern(R"(^rename node)");
//         regex rename_node_full_pattern(R"(^rename node (\w+) (\w+)$)");
//         if (regex_match(in, matches, rename_node_full_pattern)) { controller.handleRenameNodeCommand(matches[1].str(), matches[2].str()); return true;
//         } else if (regex_search(in, rename_node_base_pattern)) {
//             throw logic_error("ERROR: Invalid syntax correct format: rename node <old_name> <new_name>");
//         }
//         return false;
//     }
//
//     bool tryParseListCommands(const string& in) {
//         smatch matches;
//         regex list_all_pattern(R"(^list$)");
//         if (regex_match(in, matches, list_all_pattern)) { controller.handleListCommand(); return true; }
//         regex list_comp_pattern(R"(^list ([RCLDVIEGHF])$)");
//         if (regex_match(in, matches, list_comp_pattern)) { controller.handleListCommand(matches[1].str()); return true; }
//         return false;
//     }
//
//     bool tryParsePrintCommands(const string& in) {
//         smatch matches;
//         regex print_base_pattern(R"(^print\s+(TRAN|DC|AC)\s*(.*)$)");
//         if (regex_match(in, matches, print_base_pattern)) {
//             string analysisType = matches[1].str();
//             string remaining_args = trim(matches[2].str()); // Renamed from باقی_args
//             vector<string> analysisParams;
//             string outputVarsStr;
//
//             if (analysisType == "TRAN") {
//                 regex tran_params_regex(R"(^([^\s]+)\s+([^\s]+)(?:\s+([^\s]+))?(?:\s+([^\s]+))?\s*(.*)$)");
//                 smatch tran_param_matches;
//                 if (regex_match(remaining_args, tran_param_matches, tran_params_regex)) {
//                     string tstep = tran_param_matches[1].str();
//                     string tstop = tran_param_matches[2].str();
//                     if ((checkDouble(valuate(tstep).first) || checkingNemadElmi(valuate(tstep).first)) &&
//                         (checkDouble(valuate(tstop).first) || checkingNemadElmi(valuate(tstop).first)))
//                     {
//                         validateTimeParameter("Tstep", tstep, "print TRAN", false);
//                         validateTimeParameter("Tstop", tstop, "print TRAN", false);
//                          if (stod(valuate(tstop).first) <= stod(valuate(tstep).first)) {
//                             throw logic_error("Error: Tstop must be greater than Tstep in print TRAN.");
//                         }
//                         analysisParams.push_back(tstep);
//                         analysisParams.push_back(tstop);
//                         if (tran_param_matches[3].matched) {
//                              validateTimeParameter("Tstart", tran_param_matches[3].str(), "print TRAN");
//                              analysisParams.push_back(tran_param_matches[3].str());
//                         }
//                         if (tran_param_matches[4].matched) {
//                             validateTimeParameter("Tmaxstep", tran_param_matches[4].str(), "print TRAN", false);
//                             analysisParams.push_back(tran_param_matches[4].str());
//                         }
//                         outputVarsStr = trim(tran_param_matches[5].str());
//                     } else {
//                         outputVarsStr = remaining_args;
//                     }
//                 } else {
//                     outputVarsStr = remaining_args;
//                 }
//             } else if (analysisType == "DC") {
//                 regex dc_params_regex(R"(^([VI]\w*)\s+([^\s]+)\s+([^\s]+)\s+([^\s]+)\s*(.*)$)");
//                 smatch dc_param_matches;
//                  if (regex_match(remaining_args, dc_param_matches, dc_params_regex)) {
//                     string src = dc_param_matches[1].str();
//                     string start = dc_param_matches[2].str();
//                     string end_v = dc_param_matches[3].str();
//                     string inc = dc_param_matches[4].str();
//                      if ((checkDouble(valuate(start).first) || checkingNemadElmi(valuate(start).first)) &&
//                         (checkDouble(valuate(end_v).first) || checkingNemadElmi(valuate(end_v).first)) &&
//                         (checkDouble(valuate(inc).first) || checkingNemadElmi(valuate(inc).first)))
//                     {
//                         validateNumericParameter("StartValue", start, "print DC for " + src);
//                         validateNumericParameter("EndValue", end_v, "print DC for " + src);
//                         validateNumericParameter("Increment", inc, "print DC for " + src, false);
//                         analysisParams.push_back(src);
//                         analysisParams.push_back(start);
//                         analysisParams.push_back(end_v);
//                         analysisParams.push_back(inc);
//                         outputVarsStr = trim(dc_param_matches[5].str());
//                     } else {
//                         outputVarsStr = remaining_args;
//                     }
//                 } else {
//                     outputVarsStr = remaining_args;
//                 }
//             } else {
//                 outputVarsStr = remaining_args;
//             }
//
//             if (outputVarsStr.empty() && (analysisType == "TRAN" || analysisType == "DC") && analysisParams.empty()) {
//                  throw logic_error("Error: Missing output variables for print " + analysisType + " command.");
//             }
//              if (outputVarsStr.empty() && !analysisParams.empty()) {
//                  throw logic_error("Error: Missing output variables for print " + analysisType + " command (analysis params were provided).");
//             }
//             try {
//                 controller.handlePrintCommand(analysisType, analysisParams, outputVarsStr);
//             } catch (const logic_error& e) {
//                 string err_msg = e.what();
//                 if (err_msg.find("not found in circuit") != string::npos ) { throw; }
//                 throw logic_error("Syntax error in command");
//             }
//             return true;
//         }
//         return false;
//     }
//
//     bool tryParseFileAndMenuCommands(const string& in) {
//         smatch matches;
//         regex new_file_pattern(R"(^NewFile\s+(.+)$)");
//         if (regex_match(in, matches, new_file_pattern)) { controller.handleNewFileCommand(matches[1].str()); return true;}
//         regex show_schematics_pattern(R"(^show existing schematics$)");
//         if (regex_match(in, matches, show_schematics_pattern)) { controller.handleShowExistingSchematicsCommand(); return true;}
//         if (controller.is_expecting_schematic_choice) { controller.handleChooseSchematic(in); return true; }
//         return false;
//     }
//
// public:
//     explicit CommandParser(centralController& ctrl) : controller(ctrl) {}
//
//     void processInput(const string& in) {
//         if (in.empty()) { return; }
//         if (tryParseFileAndMenuCommands(in)) return;
//         if (tryParseAnalysisDefinitionCommands(in)) return;
//         if (tryParseNodeCommands(in)) return;
//         if (tryParseListCommands(in)) return;
//         if (tryParsePrintCommands(in)) return;
//         if (tryParseElementCommands(in)) return;
//         if (tryParseSourceCommands(in)) return;
//         throw logic_error("Error: Unknown command or invalid syntax. -> " + in);
//     }
// };
//
// int main() {
//     centralController controller_instance;
//     CommandParser parser(controller_instance);
//
//     vector<string> test_commands = {
//         // --- Section 13 Elements ---
//         "add R1 N1 N2 1k",
//         // --- Section 18 Independent Sources (DC with keywords, Vname for SIN/PULSE/DC) ---
//         "add VoltageSourceVS1 N_plus N_minus 5",
//         "add CurrentSourceIS1 N_from N_to 0.5m",
//         "add Vac N_in 0 SIN(0,5,1k)",
//         "add Vclk clk 0 PULSE(0 1 1n 0.1n 0.1n 5n 10n)",
//         "add Vdc N1 N2 3.3",
//         // --- Section 18 Dependent Sources ---
//         "add Eamp Nout Nin Nctrl1 Nctrl2 100",
//         // --- Section 20: Analysis Definition ---
//         ".TRAN 1n 100n",
//         ".TRAN 1p 1u 0.1u",
//         ".TRAN 1u 1m 0 10n",
//         ".TRAN 1m 0.1m",
//         ".TRAN 1n abc",
//         ".DC Vdc 0 5 0.1",
//         ".DC Isrc -1m 1m 0.01m",
//         ".DC V_unknown 0 1 0.1",
//         ".DC Vdc 0 5 0",
//         // --- Section 15: Print Commands (refined) ---
//         "print TRAN V(N1) I(R1)",
//         "print TRAN 1p 50p V(N_plus) V(N_minus)",
//         "print TRAN 1n 200n 10n V(clk)",
//         "print TRAN 0.1u 10u 1u 0.5u I(Vac)",
//         "print TRAN 1u V(N1)",
//         "print TRAN V(N1) 1u 10u",
//         "print DC V(N_plus)",
//         "print DC VoltageSourceVS1 -2 2 0.05 V(N_from) I(IS1)",
//         "print DC Vdc V(N1)",
//         "print AC V(N1)",
//         // --- Other Commands ---
//         "nodes", "list R", "rename node N001 Vout",
//         // --- Error Cases for elements/sources (some might be caught by generic unknown command) ---
//         "add R_err N1 N2 0ohm",
//         "add VoltageSourceBadVal V_bat N1 0 -9.0X",
//         "add Vpulse_err_params N1 N2 PULSE(0 1 1n -0.1n 0.1n 5n 10n)",
//         "add E_no_gain N1 N2 N3 N4",
//         "delete SomeRandomName"
//     };
//
//     cout << "--- Starting Command Tests ---" << endl;
//     for (const string& cmd : test_commands) {
//         cout << "\nInput: \"" << cmd << "\"" << endl;
//         try {
//             parser.processInput(cmd);
//         } catch (const logic_error& e) {
//             cout << "Exception: " << e.what() << endl;
//         }
//     }
//     cout << "\n--- Testing Schematic Choice Menu ---" << endl;
//     string menu_inputs[] = {"show existing schematics", "5", "draft2", "1", "return"};
//     for (const string& cmd : menu_inputs) {
//         cout << "\nInput: \"" << cmd << "\"" << endl;
//         try {
//             parser.processInput(cmd);
//         } catch (const logic_error& e) {
//             cout << "Exception: " << e.what() << endl;
//         }
//     }
//     return 0;
// }

//////////////////////////////////////////////////////////


//
// #include <iostream>
// #include <vector>
// #include <string>
// #include <stdexcept>
// #include <map>
// #include <cmath>
// #include <regex>
// #include <algorithm>
// #include <iomanip>
// #include "Eigen/Dense" // کتابخانه Eigen برای ماتریس‌ها
//
// #ifndef M_PI
// #define M_PI 3.14159265358979323846
// #endif
//
// using namespace std;
//
// // ==========================================================================================
// // ||                                    بخش مدل (Model)                                   ||
// // || این بخش شامل کلاس‌های مربوط به منطق اصلی مدار، المان‌ها و تحلیل‌گرها می‌باشد.         ||
// // ==========================================================================================
//
// /**
//  * @class Node
//  * @brief کلاسی برای نمایش یک گره در مدار الکتریکی.
//  *
//  * این کلاس اطلاعات مربوط به نام، ولتاژ فعلی و ولتاژ در گام زمانی قبلی را نگهداری می‌کند.
//  */
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
//     double getVoltage() const { return voltage; }
//     void setVoltage(double v) { voltage = v; }
//     double getPreviousVoltage() const { return previousVoltage; }
//     void setPreviousVoltage(double pv) { previousVoltage = pv; }
//     void updateVoltageForNextStep() { previousVoltage = voltage; }
//     bool isGround() const { return name == "0" || name == "GND" || name == "gnd"; }
// };
//
// /**
//  * @class Element
//  * @brief کلاس پایه و انتزاعی برای تمام المان‌های مداری.
//  *
//  * تمام المان‌های دیگر از این کلاس ارث‌بری می‌کنند.
//  */
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
//     string getType() const override { return "Resistor"; }
//     double getValue() const override { return resistance; }
//     void setResistance(double res) { if (res > 0) this->resistance = res; }
//     double getCurrent() const override { return (node1->getVoltage() - node2->getVoltage()) / resistance; }
// };
//
// class VoltageSource : public Element {
// public:
//     enum SourceType { DC, SIN, PULSE };
// private:
//     double value;
//     double currentThroughSource;
//     SourceType sourceType;
//     // پارامترهای منابع سینوسی و پالسی
//     double dcOffset, amplitude, frequency;
//     double initialValue, pulsedValue, delayTime, riseTime, fallTime, onTime, period;
// public:
//     VoltageSource(Node* n1, Node* n2, const string &name, double dc_val)
//             : Element(n1, n2, name), value(dc_val), currentThroughSource(0.0), sourceType(DC) {}
//
//     void setSinParams(double offset, double amp, double freq) {
//         sourceType = SIN;
//         dcOffset = offset;
//         amplitude = amp;
//         frequency = freq > 0 ? freq : 0;
//     }
//
//     void setPulseParams(double v1, double v2, double td, double tr, double tf, double ton, double tper) {
//         sourceType = PULSE;
//         initialValue = v1;
//         pulsedValue = v2;
//         delayTime = td >= 0 ? td : 0;
//         riseTime = tr > 0 ? tr : 1e-9;
//         fallTime = tf > 0 ? tf : 1e-9;
//         onTime = ton >= 0 ? ton : 0;
//         if (tper <= 0) throw std::invalid_argument("Pulse period for " + name + " must be positive.");
//         period = tper;
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
//                 value = pulsedValue - (pulsedValue - initialValue) * ((timeInCycle - riseTime - onTime) / fallTime);
//             } else {
//                 value = initialValue;
//             }
//         }
//     }
//
//     void setVoltageValue(double val) {
//         sourceType = DC;
//         value = val;
//     }
//     string getType() const override { return "VoltageSource"; }
//     double getValue() const override { return value; }
//     double getCurrent() const override { return currentThroughSource; }
//     void setCurrent(double current) override { this->currentThroughSource = current; }
// };
//
// class CurrentSource : public Element {
// private: double currentValue;
// public:
//     CurrentSource(Node* n1, Node* n2, const string &name, double val) : Element(n1, n2, name), currentValue(val) {}
//     string getType() const override { return "CurrentSource"; }
//     double getValue() const override { return currentValue; }
// };
//
// class Capacitor : public Element {
// private: double capacitance;
// public:
//     Capacitor(Node* n1, Node* n2, const string &name, double cap) : Element(n1, n2, name) {
//         if (cap <= 0) throw std::invalid_argument("Capacitance must be positive for " + name);
//         this->capacitance = cap;
//     }
//     string getType() const override { return "Capacitor"; }
//     double getValue() const override { return capacitance; }
// };
//
// class Inductor : public Element {
// private: double inductance, current, previousCurrent;
// public:
//     Inductor(Node* n1, Node* n2, const string &name, double ind)
//             : Element(n1, n2, name), inductance(ind), current(0.0), previousCurrent(0.0) {
//         if (ind <= 0) throw std::invalid_argument("Inductance must be positive for " + name);
//     }
//     string getType() const override { return "Inductor"; }
//     double getValue() const override { return inductance; }
//     double getCurrent() const override { return current; }
//     void setCurrent(double c) override { current = c; }
//     double getPreviousCurrent() const { return previousCurrent; }
//     void updateCurrentForNextStep() { previousCurrent = current; }
// };
//
// class IdealDiode : public Element {
// public: enum State { ON, OFF };
// private: double forwardVoltage; State currentState; double current;
// public:
//     IdealDiode(Node* n1, Node* n2, const string& name, double vf)
//             : Element(n1, n2, name), forwardVoltage(vf), currentState(OFF), current(0.0) {
//         if (vf < 0) throw std::invalid_argument("Diode forward voltage must be non-negative for " + name);
//     }
//     string getType() const override { return "IdealDiode"; }
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
//     string getType() const override { return "VCVS"; }
//     double getValue() const override { return gain; }
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
//     string getType() const override { return "VCCS"; }
//     double getValue() const override { return gain; }
//     Node* getControlNode1() const { return controlNode1; }
//     Node* getControlNode2() const { return controlNode2; }
// };
//
// class CCVS : public Element {
// private: string controlVoltageSourceName; double gain; double current;
// public:
//     CCVS(Node* n1, Node* n2, const string& name, const string& cvs_name, double g)
//             : Element(n1, n2, name), controlVoltageSourceName(cvs_name), gain(g), current(0.0) {}
//     string getType() const override { return "CCVS"; }
//     double getValue() const override { return gain; }
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
//     string getType() const override { return "CCCS"; }
//     double getValue() const override { return gain; }
//     string getControlVoltageSourceName() const { return controlVoltageSourceName; }
// };
//
// /**
//  * @class MakingMNA
//  * @brief کلاسی برای مدیریت مدار، ساخت ماتریس‌های MNA و بردار Z.
//  */
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
//         if (!groundNodeRef) throw std::runtime_error("Error: Ground node not detected.");
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
//     double getTimeStep() const { return this->timeStep_h; }
//     Element* getElement(const string& name) {
//         for (auto* elem : elementsInCircuit) if (elem->getName() == name) return elem;
//         return nullptr;
//     }
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
//                 if (!n1->isGround()) A(nodeToIndexMap[n1], nodeToIndexMap[n1]) += g;
//                 if (!n2->isGround()) A(nodeToIndexMap[n2], nodeToIndexMap[n2]) += g;
//                 if (!n1->isGround() && !n2->isGround()) {
//                     A(nodeToIndexMap[n1], nodeToIndexMap[n2]) -= g;
//                     A(nodeToIndexMap[n2], nodeToIndexMap[n1]) -= g;
//                 }
//             }
//             else if (auto cap = dynamic_cast<Capacitor*>(elem)) {
//                 if (!isDCAnalysis) {
//                     if (this->timeStep_h <= 0) throw std::runtime_error("Time step h is not set for capacitor " + cap->getName());
//                     double g = cap->getValue() / this->timeStep_h;
//                     if (!n1->isGround()) A(nodeToIndexMap[n1], nodeToIndexMap[n1]) += g;
//                     if (!n2->isGround()) A(nodeToIndexMap[n2], nodeToIndexMap[n2]) += g;
//                     if (!n1->isGround() && !n2->isGround()) {
//                         A(nodeToIndexMap[n1], nodeToIndexMap[n2]) -= g;
//                         A(nodeToIndexMap[n2], nodeToIndexMap[n1]) -= g;
//                     }
//                 }
//             }
//             else if (auto vs = dynamic_cast<VoltageSource*>(elem)) {
//                 int i = numNonGroundNodes + vsToIndexMap[vs];
//                 if (!n1->isGround()) { A(nodeToIndexMap[n1], i) += 1.0; A(i, nodeToIndexMap[n1]) += 1.0; }
//                 if (!n2->isGround()) { A(nodeToIndexMap[n2], i) -= 1.0; A(i, nodeToIndexMap[n2]) -= 1.0; }
//             }
//             else if (auto ind = dynamic_cast<Inductor*>(elem)) {
//                 int i = numNonGroundNodes + numVS + inductorToIndexMap[ind];
//                 if (isDCAnalysis) { // در تحلیل DC، سلف اتصال کوتاه است
//                     if (!n1->isGround()) A(i, nodeToIndexMap[n1]) += 1.0;
//                     if (!n2->isGround()) A(i, nodeToIndexMap[n2]) -= 1.0;
//                 } else { // در تحلیل گذرا
//                     double l_div_h = ind->getValue() / this->timeStep_h;
//                     if (!n1->isGround()) { A(nodeToIndexMap[n1], i) += 1.0; A(i, nodeToIndexMap[n1]) += 1.0; }
//                     if (!n2->isGround()) { A(nodeToIndexMap[n2], i) -= 1.0; A(i, nodeToIndexMap[n2]) -= 1.0; }
//                     A(i, i) -= l_div_h;
//                 }
//             }
//             else if (auto diode = dynamic_cast<IdealDiode*>(elem)) {
//                 int i = numNonGroundNodes + numVS + numL + idealDiodeToIndexMap[diode];
//                 if (diode->getState() == IdealDiode::ON) {
//                     if (!n1->isGround()) { A(nodeToIndexMap[n1], i) += 1.0; A(i, nodeToIndexMap[n1]) += 1.0; }
//                     if (!n2->isGround()) { A(nodeToIndexMap[n2], i) -= 1.0; A(i, nodeToIndexMap[n2]) -= 1.0; }
//                 } else { // OFF State
//                     A(i, i) = 1.0;
//                 }
//             }
//             else if (auto vccs = dynamic_cast<VCCS*>(elem)) {
//                 double g = vccs->getValue();
//                 Node* cn1 = vccs->getControlNode1(); Node* cn2 = vccs->getControlNode2();
//                 if (!n1->isGround() && !cn1->isGround()) A(nodeToIndexMap[n1], nodeToIndexMap[cn1]) += g;
//                 if (!n1->isGround() && !cn2->isGround()) A(nodeToIndexMap[n1], nodeToIndexMap[cn2]) -= g;
//                 if (!n2->isGround() && !cn1->isGround()) A(nodeToIndexMap[n2], nodeToIndexMap[cn1]) -= g;
//                 if (!n2->isGround() && !cn2->isGround()) A(nodeToIndexMap[n2], nodeToIndexMap[cn2]) += g;
//             }
//             else if (auto cccs = dynamic_cast<CCCS*>(elem)) {
//                 double gain = cccs->getValue();
//                 VoltageSource* ctrl_vs = nullptr;
//                 for(auto* vs_ptr : orderedVoltageSources) if(vs_ptr->getName() == cccs->getControlVoltageSourceName()) ctrl_vs = vs_ptr;
//                 if (!ctrl_vs) throw std::runtime_error("Error: Dependent source '" + cccs->getName() + "' has an undefined control element '" + cccs->getControlVoltageSourceName() + "'.");
//                 int ctrl_i = numNonGroundNodes + vsToIndexMap[ctrl_vs];
//                 if (!n1->isGround()) A(nodeToIndexMap[n1], ctrl_i) += gain;
//                 if (!n2->isGround()) A(nodeToIndexMap[n2], ctrl_i) -= gain;
//             }
//             else if (auto vcvs = dynamic_cast<VCVS*>(elem)) {
//                 int i = numNonGroundNodes + numVS + numL + numD + vcvsToIndexMap[vcvs];
//                 Node* cn1 = vcvs->getControlNode1(); Node* cn2 = vcvs->getControlNode2();
//                 double gain = vcvs->getValue();
//                 if (!n1->isGround()) { A(nodeToIndexMap[n1], i) += 1.0; A(i, nodeToIndexMap[n1]) += 1.0; }
//                 if (!n2->isGround()) { A(nodeToIndexMap[n2], i) -= 1.0; A(i, nodeToIndexMap[n2]) -= 1.0; }
//                 if (!cn1->isGround()) A(i, nodeToIndexMap[cn1]) -= gain;
//                 if (!cn2->isGround()) A(i, nodeToIndexMap[cn2]) += gain;
//             }
//             else if (auto ccvs = dynamic_cast<CCVS*>(elem)) {
//                 int i = numNonGroundNodes + numVS + numL + numD + numVCVS + ccvsToIndexMap[ccvs];
//                 double gain = ccvs->getValue();
//                 VoltageSource* ctrl_vs = nullptr;
//                 for(auto* vs_ptr : orderedVoltageSources) if(vs_ptr->getName() == ccvs->getControlVoltageSourceName()) ctrl_vs = vs_ptr;
//                 if (!ctrl_vs) throw std::runtime_error("Error: Dependent source '" + ccvs->getName() + "' has an undefined control element '" + ccvs->getControlVoltageSourceName() + "'.");
//                 int ctrl_i = numNonGroundNodes + vsToIndexMap[ctrl_vs];
//                 if (!n1->isGround()) {A(i, nodeToIndexMap[n1]) += 1.0;} else {A(i, nodeToIndexMap[n1])=0;};
//                 if (!n2->isGround()) {A(i, nodeToIndexMap[n2]) -= 1.0;} else {A(i, nodeToIndexMap[n2])=0;};
//                 A(i, ctrl_i) -= gain;
//             }
//         }
//         return A;
//     }
//
//     Eigen::VectorXd getSystemVectorZ(bool isDCAnalysis = false) {
//         int systemSize = orderedNonGroundNodes.size() + orderedVoltageSources.size() + orderedInductors.size() + orderedIdealDiodes.size() + orderedVCVS.size() + orderedCCVS.size();
//         if (systemSize == 0) return Eigen::VectorXd(0);
//         Eigen::VectorXd Z = Eigen::VectorXd::Zero(systemSize);
//
//         for (Element* elem : elementsInCircuit) {
//             if (auto cs = dynamic_cast<CurrentSource*>(elem)) {
//                 if (!cs->getNode1()->isGround()) Z(nodeToIndexMap[cs->getNode1()]) -= cs->getValue();
//                 if (!cs->getNode2()->isGround()) Z(nodeToIndexMap[cs->getNode2()]) += cs->getValue();
//             } else if (auto cap = dynamic_cast<Capacitor*>(elem)) {
//                 if (!isDCAnalysis) {
//                     if (this->timeStep_h <= 0) throw std::runtime_error("Time step h is not set for capacitor " + cap->getName());
//                     double c_div_h = cap->getValue() / this->timeStep_h;
//                     double v_n1_prev = cap->getNode1()->getPreviousVoltage();
//                     double v_n2_prev = cap->getNode2()->getPreviousVoltage();
//                     double i_eq_cap = c_div_h * (v_n1_prev - v_n2_prev);
//                     if (!cap->getNode1()->isGround()) Z(nodeToIndexMap[cap->getNode1()]) += i_eq_cap;
//                     if (!cap->getNode2()->isGround()) Z(nodeToIndexMap[cap->getNode2()]) -= i_eq_cap;
//                 }
//             }
//         }
//
//         for (const auto& vs : orderedVoltageSources) Z(orderedNonGroundNodes.size() + vsToIndexMap[vs]) = vs->getValue();
//         for (const auto& ind : orderedInductors) {
//             int i = orderedNonGroundNodes.size() + orderedVoltageSources.size() + inductorToIndexMap[ind];
//             if (!isDCAnalysis) {
//                 Z(i) = - (ind->getValue() / this->timeStep_h) * ind->getPreviousCurrent();
//             }
//         }
//         for (const auto& diode : orderedIdealDiodes) {
//             int i = orderedNonGroundNodes.size() + orderedVoltageSources.size() + orderedInductors.size() + idealDiodeToIndexMap[diode];
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
//         if (A.rows() > 0 && std::abs(lu.determinant()) < 1e-14) {
//              throw std::runtime_error("Error: System matrix is singular or ill-conditioned. The circuit may not be solvable.");
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
// class TransientAnalysis {
// private:
//     MakingMNA& mnaCircuit;
//     MNASolver solver;
//     double t_step, t_stop;
// public:
//     TransientAnalysis(MakingMNA& circuit, double step, double stop)
//             : mnaCircuit(circuit), solver(), t_step(step), t_stop(stop) {
//         if (t_step <= 0 || t_stop <= 0 || t_step > t_stop) {
//             throw std::invalid_argument("Invalid time parameters for transient analysis.");
//         }
//     }
//
//     void run(const vector<string>& output_vars) {
//         double current_time = 0.0;
//
//         cout << "Time(s)\t";
//         for (const auto& var : output_vars) cout << var << "\t";
//         cout << endl;
//         cout << "----------------------------------------------------------------" << endl;
//
//         mnaCircuit.setTimeStep(t_step);
//
//         while (current_time <= t_stop) {
//             for (auto* elem : mnaCircuit.getAllElements()) {
//                 elem->updateValue(current_time);
//             }
//
//             bool diodes_converged = false;
//             int max_diode_iterations = 25;
//             int iteration_count = 0;
//             while (!diodes_converged && iteration_count < max_diode_iterations) {
//                  iteration_count++;
//                 diodes_converged = true;
//
//                 Eigen::MatrixXd A = mnaCircuit.getSystemMatrixA();
//                 Eigen::VectorXd Z = mnaCircuit.getSystemVectorZ();
//                 Eigen::VectorXd X = solver.solve(A, Z);
//                 solver.updateCircuitState(X, mnaCircuit);
//
//                 for (auto* diode_ptr : mnaCircuit.getOrderedIdealDiodes()) {
//                     IdealDiode::State old_state = diode_ptr->getState();
//                     double v_anode = diode_ptr->getNode1()->getVoltage();
//                     double v_cathode = diode_ptr->getNode2()->getVoltage();
//                     if (old_state == IdealDiode::OFF) {
//                         if (v_anode > v_cathode + diode_ptr->getForwardVoltage()) {
//                             diode_ptr->setState(IdealDiode::ON);
//                             diodes_converged = false;
//                         }
//                     } else { // ON State
//                         if (diode_ptr->getCurrent() < 0) {
//                             diode_ptr->setState(IdealDiode::OFF);
//                             diodes_converged = false;
//                         }
//                     }
//                 }
//             }
//             if (!diodes_converged) cout << "Warning: Diode states failed to converge at t=" << current_time << endl;
//
//             // چاپ نتایج
//             cout << fixed << setprecision(6) << current_time << "\t";
//             for (const auto& var : output_vars) {
//                  if (var[0] == 'V' && var[1] == '(') {
//                     string node_name = var.substr(2, var.length() - 3);
//                     bool found = false;
//                     for (auto* node : mnaCircuit.getAllNodesInCircuit()) {
//                         if (node->getName() == node_name) {
//                             cout << node->getVoltage() << "\t";
//                             found = true;
//                             break;
//                         }
//                     }
//                     if (!found) cout << "NaN\t";
//                 } else if (var[0] == 'I' && var[1] == '(') {
//                     string elem_name = var.substr(2, var.length() - 3);
//                     Element* elem = mnaCircuit.getElement(elem_name);
//                     if (elem) {
//                         cout << elem->getCurrent() << "\t";
//                     } else {
//                         cout << "NaN\t";
//                     }
//                 }
//             }
//             cout << endl;
//
//             current_time += t_step;
//
//             for (auto* node : mnaCircuit.getAllNodesInCircuit()) node->updateVoltageForNextStep();
//             for (auto* ind : mnaCircuit.getOrderedInductors()) ind->updateCurrentForNextStep();
//         }
//     }
// };
//
// class DCSweepAnalysis {
// private:
//     MakingMNA& mnaCircuit;
//     MNASolver solver;
//     string sweepComponentName;
//     double startValue, endValue, increment;
// public:
//     DCSweepAnalysis(MakingMNA& circuit, const string& compName, double start, double end, double inc)
//             : mnaCircuit(circuit), solver(), sweepComponentName(compName), startValue(start), endValue(end), increment(inc) {
//         if (increment == 0) throw std::invalid_argument("DC sweep increment cannot be zero.");
//         if ((endValue > startValue && increment < 0) || (endValue < startValue && increment > 0)) {
//             increment = -increment;
//         }
//     }
//     void run(const vector<string>& output_vars) {
//         Element* sweepElement = mnaCircuit.getElement(sweepComponentName);
//         if (!sweepElement) throw std::runtime_error("Sweep component '" + sweepComponentName + "' not found.");
//         auto* sweepVoltageSource = dynamic_cast<VoltageSource*>(sweepElement);
//         if (!sweepVoltageSource) throw std::runtime_error("Sweep component must be a VoltageSource.");
//
//         cout << sweepComponentName << "\t";
//         for (const auto& var : output_vars) cout << var << "\t";
//         cout << endl;
//         cout << "-----------------------------------------------------" << endl;
//
//         double sweep_condition = (increment > 0) ? startValue : endValue;
//         double end_condition = (increment > 0) ? endValue : startValue;
//
//         for (double val = startValue; (increment > 0 ? val <= endValue : val >= endValue) ; val += increment) {
//             sweepVoltageSource->setVoltageValue(val);
//
//             Eigen::MatrixXd A = mnaCircuit.getSystemMatrixA(true);
//             Eigen::VectorXd Z = mnaCircuit.getSystemVectorZ(true);
//             Eigen::VectorXd X = solver.solve(A, Z);
//             solver.updateCircuitState(X, mnaCircuit);
//
//             // چاپ نتایج
//             cout << fixed << setprecision(6) << val << "\t";
//             for (const auto& var : output_vars) {
//                  if (var[0] == 'V' && var[1] == '(') {
//                     string node_name = var.substr(2, var.length() - 3);
//                     bool found = false;
//                     for (auto* node : mnaCircuit.getAllNodesInCircuit()) {
//                         if (node->getName() == node_name) {
//                             cout << node->getVoltage() << "\t";
//                             found = true;
//                             break;
//                         }
//                     }
//                     if (!found) cout << "NaN\t";
//                 } else if (var[0] == 'I' && var[1] == '(') {
//                     string elem_name = var.substr(2, var.length() - 3);
//                     Element* elem = mnaCircuit.getElement(elem_name);
//                     if (elem) {
//                         cout << elem->getCurrent() << "\t";
//                     } else {
//                         cout << "NaN\t";
//                     }
//                 }
//             }
//             cout << endl;
//         }
//     }
// };
//
// // ============================================================================================
// // ||                  بخش کنترلر و نمایش (Controller & View)                                 ||
// // || این بخش شامل کلاس‌های مربوط به پردازش دستورات کاربر و ارتباط با مدل می‌باشد.               ||
// // ============================================================================================
//
// // توابع کمکی برای پردازش ورودی
// bool checkDouble(const string& s);
// bool checkingNemadElmi(const string& s);
// pair<string, string> valuate(const string& s);
// double stringToDouble(const string& s);
// string trim(const string& str);
//
// // تعریف ساختار برای نگهداری پارامترهای تحلیل
// struct AnalysisParams {
//     string type; // "TRAN" or "DC"
//     vector<string> params;
// };
//
// /**
//  * @class centralController
//  * @brief کلاس کنترلر مرکزی برای مدیریت تمام عملیات شبیه‌ساز.
//  *
//  * این کلاس به عنوان واسط بین ورودی کاربر (View) و منطق مدار (Model) عمل می‌کند.
//  * وظیفه ساخت، حذف و مدیریت المان‌ها، گره‌ها و اجرای تحلیل‌ها را بر عهده دارد.
//  */
// class centralController {
// private:
//     MakingMNA circuit_manager;
//     map<string, Node*> nodes_map;
//     vector<Element*> elements_list;
//     map<string, Element*> elements_map;
//     AnalysisParams last_analysis;
//
//     // متد کمکی برای دریافت یا ایجاد گره
//     Node* getNode(const string& name) {
//         if (nodes_map.find(name) == nodes_map.end()) {
//             nodes_map[name] = new Node(name);
//         }
//         return nodes_map[name];
//     }
//
// public:
//     centralController() {}
//
//     ~centralController() {
//         for (auto const& [name, node_ptr] : nodes_map) {
//             delete node_ptr;
//         }
//         for (auto elem_ptr : elements_list) {
//             delete elem_ptr;
//         }
//     }
//
//     // --- مدیریت المان‌ها ---
//     void makingResistor(const string& name, const string& n1_name, const string& n2_name, const string& val_str) {
//         if (elements_map.count(name)) throw logic_error("Error: Resistor " + name + " already exists in the circuit");
//         double resistance = stringToDouble(val_str);
//         Node* n1 = getNode(n1_name);
//         Node* n2 = getNode(n2_name);
//         Resistor* res = new Resistor(n1, n2, name, resistance);
//         elements_list.push_back(res);
//         elements_map[name] = res;
//         cout << "SUCCESS: Resistor " << name << " added." << endl;
//     }
//
//     void makingCapacity(const string& name, const string& n1_name, const string& n2_name, const string& val_str) {
//         if (elements_map.count(name)) throw logic_error("Error: Capacitor " + name + " already exists in the circuit");
//         double capacitance = stringToDouble(val_str);
//         Node* n1 = getNode(n1_name);
//         Node* n2 = getNode(n2_name);
//         Capacitor* cap = new Capacitor(n1, n2, name, capacitance);
//         elements_list.push_back(cap);
//         elements_map[name] = cap;
//         cout << "SUCCESS: Capacitor " << name << " added." << endl;
//     }
//
//     void makingInductor(const string& name, const string& n1_name, const string& n2_name, const string& val_str) {
//         if (elements_map.count(name)) throw logic_error("Error: Inductor " + name + " already exists in the circuit");
//         double inductance = stringToDouble(val_str);
//         Node* n1 = getNode(n1_name);
//         Node* n2 = getNode(n2_name);
//         Inductor* ind = new Inductor(n1, n2, name, inductance);
//         elements_list.push_back(ind);
//         elements_map[name] = ind;
//         cout << "SUCCESS: Inductor " << name << " added." << endl;
//     }
//
//     void makingDiode(const string& name, const string& n1_name, const string& n2_name, const string& model) {
//         if (elements_map.count(name)) throw logic_error("Error: Diode " + name + " already exists in the circuit");
//         Node* n1 = getNode(n1_name);
//         Node* n2 = getNode(n2_name);
//         IdealDiode* diode = new IdealDiode(n1, n2, name, 0.7); // Vf=0.7 for standard diode
//         elements_list.push_back(diode);
//         elements_map[name] = diode;
//         cout << "SUCCESS: Diode " << name << " with model " << model << " added." << endl;
//     }
//
//     // --- مدیریت منابع ---
//     void makingVoltageSourceDC(const string& name, const string& n_plus, const string& n_minus, const string& val_str) {
//         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
//         double voltage = stringToDouble(val_str);
//         Node* n1 = getNode(n_plus);
//         Node* n2 = getNode(n_minus);
//         VoltageSource* vs = new VoltageSource(n1, n2, name, voltage);
//         elements_list.push_back(vs);
//         elements_map[name] = vs;
//         cout << "SUCCESS: DC VoltageSource " << name << " added." << endl;
//     }
//
//     void makingCurrentSourceDC(const string& name, const string& n_plus, const string& n_minus, const string& val_str) {
//         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
//         double current = stringToDouble(val_str);
//         Node* n1 = getNode(n_plus);
//         Node* n2 = getNode(n_minus);
//         CurrentSource* cs = new CurrentSource(n1, n2, name, current);
//         elements_list.push_back(cs);
//         elements_map[name] = cs;
//         cout << "SUCCESS: DC CurrentSource " << name << " added." << endl;
//     }
//
//     void makingVoltageSourceSin(const string& name, const string& n_plus, const string& n_minus, const string& v_off, const string& v_amp, const string& freq) {
//          if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
//         Node* n1 = getNode(n_plus);
//         Node* n2 = getNode(n_minus);
//         VoltageSource* vs = new VoltageSource(n1, n2, name, 0.0);
//         vs->setSinParams(stringToDouble(v_off), stringToDouble(v_amp), stringToDouble(freq));
//         elements_list.push_back(vs);
//         elements_map[name] = vs;
//         cout << "SUCCESS: Sinusoidal Voltage Source " << name << " added." << endl;
//     }
//
//     void makingVoltageSourcePulse(const string& name, const string& n_plus, const string& n_minus,
//                                   const string& v1, const string& v2, const string& td,
//                                   const string& tr, const string& tf, const string& pw,
//                                   const string& period) {
//         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
//         Node* n1 = getNode(n_plus);
//         Node* n2 = getNode(n_minus);
//         VoltageSource* vs = new VoltageSource(n1, n2, name, 0.0);
//         vs->setPulseParams(stringToDouble(v1), stringToDouble(v2), stringToDouble(td), stringToDouble(tr), stringToDouble(tf), stringToDouble(pw), stringToDouble(period));
//         elements_list.push_back(vs);
//         elements_map[name] = vs;
//         cout << "SUCCESS: PULSE Voltage Source " << name << " added." << endl;
//     }
//
//     void makingVCVS(const string& name, const string& n_p, const string& n_m, const string& nc_p, const string& nc_m, const string& gain_str) {
//         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
//         double gain = stringToDouble(gain_str);
//         VCVS* vcvs = new VCVS(getNode(n_p), getNode(n_m), name, getNode(nc_p), getNode(nc_m), gain);
//         elements_list.push_back(vcvs);
//         elements_map[name] = vcvs;
//         cout << "SUCCESS: VCVS " << name << " added." << endl;
//     }
//
//     void makingVCCS(const string& name, const string& n_p, const string& n_m, const string& nc_p, const string& nc_m, const string& gain_str) {
//          if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
//         double gain = stringToDouble(gain_str);
//         VCCS* vccs = new VCCS(getNode(n_p), getNode(n_m), name, getNode(nc_p), getNode(nc_m), gain);
//         elements_list.push_back(vccs);
//         elements_map[name] = vccs;
//         cout << "SUCCESS: VCCS " << name << " added." << endl;
//     }
//
//     void makingCCVS(const string& name, const string& n_p, const string& n_m, const string& vctrl_name, const string& gain_str) {
//         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
//         if (!elements_map.count(vctrl_name) || dynamic_cast<VoltageSource*>(elements_map[vctrl_name]) == nullptr) {
//             throw logic_error("Error: Control voltage source " + vctrl_name + " not found for CCVS " + name);
//         }
//         double gain = stringToDouble(gain_str);
//         CCVS* ccvs = new CCVS(getNode(n_p), getNode(n_m), name, vctrl_name, gain);
//         elements_list.push_back(ccvs);
//         elements_map[name] = ccvs;
//         cout << "SUCCESS: CCVS " << name << " added." << endl;
//     }
//
//     void makingCCCS(const string& name, const string& n_p, const string& n_m, const string& vctrl_name, const string& gain_str) {
//         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
//         if (!elements_map.count(vctrl_name) || dynamic_cast<VoltageSource*>(elements_map[vctrl_name]) == nullptr) {
//             throw logic_error("Error: Control voltage source " + vctrl_name + " not found for CCCS " + name);
//         }
//         double gain = stringToDouble(gain_str);
//         CCCS* cccs = new CCCS(getNode(n_p), getNode(n_m), name, vctrl_name, gain);
//         elements_list.push_back(cccs);
//         elements_map[name] = cccs;
//         cout << "SUCCESS: CCCS " << name << " added." << endl;
//     }
//
//     void deletingElement(const string& name) {
//         if (!elements_map.count(name)) throw logic_error("Error: Cannot delete component; component not found");
//
//         Element* elem_to_delete = elements_map[name];
//
//         // حذف از لیست و مپ
//         elements_map.erase(name);
//         elements_list.erase(remove(elements_list.begin(), elements_list.end(), elem_to_delete), elements_list.end());
//
//         delete elem_to_delete;
//         cout << "SUCCESS: Component " << name << " deleted." << endl;
//     }
//
//     void addingGround(const string& node_name){
//         // در مدل ما، گره با نام "0" یا "GND" به طور خودکار زمین در نظر گرفته می‌شود.
//         // این تابع می‌تواند برای اطمینان از وجود گره زمین استفاده شود.
//         getNode(node_name); // اطمینان از وجود گره
//         cout << "SUCCESS: Node " << node_name << " is designated as a ground reference." << endl;
//     }
//
//     // --- مدیریت گره‌ها و لیست‌ها ---
//     void handleNodesCommand() {
//         if (nodes_map.empty()) {
//             cout << "No nodes in the circuit yet." << endl;
//             return;
//         }
//         cout << "Available nodes:" << endl;
//         for(auto const& [name, node_ptr] : nodes_map) {
//             cout << name << " ";
//         }
//         cout << endl;
//     }
//
//     void handleListCommand(const string& componentType = "") {
//         if (elements_list.empty()) {
//             cout << "No elements in the circuit yet." << endl;
//             return;
//         }
//         bool found = false;
//         cout << "Circuit elements:" << endl;
//         for(auto const& elem : elements_list) {
//             bool type_match = false;
//             if (componentType.empty()) type_match = true;
//             else if (componentType == "R" && dynamic_cast<Resistor*>(elem)) type_match = true;
//             else if (componentType == "C" && dynamic_cast<Capacitor*>(elem)) type_match = true;
//             else if (componentType == "L" && dynamic_cast<Inductor*>(elem)) type_match = true;
//             else if (componentType == "D" && dynamic_cast<IdealDiode*>(elem)) type_match = true;
//             else if (componentType == "V" && dynamic_cast<VoltageSource*>(elem)) type_match = true;
//             else if (componentType == "I" && dynamic_cast<CurrentSource*>(elem)) type_match = true;
//             else if (componentType == "E" && dynamic_cast<VCVS*>(elem)) type_match = true;
//             else if (componentType == "G" && dynamic_cast<VCCS*>(elem)) type_match = true;
//             else if (componentType == "H" && dynamic_cast<CCVS*>(elem)) type_match = true;
//             else if (componentType == "F" && dynamic_cast<CCCS*>(elem)) type_match = true;
//
//             if (type_match) {
//                 cout << "- " << elem->getName() << " (" << elem->getType() << ") connected between "
//                      << elem->getNode1()->getName() << " and " << elem->getNode2()->getName() << endl;
//                 found = true;
//             }
//         }
//         if (!found) {
//             cout << "No elements of type '" << componentType << "' found." << endl;
//         }
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
//         nodes_map.erase(oldName);
//         nodes_map[newName] = node_ptr;
//         // Note: The internal name of the Node object is not changed as it's private.
//         // The nodes_map in the controller is the single source of truth for node names.
//         cout << "SUCCESS: Node renamed from " << oldName << " to " << newName << endl;
//     }
//
//     // --- مدیریت و اجرای تحلیل ---
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
//         while (ss >> temp) {
//             output_vars.push_back(temp);
//         }
//
//         if (output_vars.empty()){
//             throw logic_error("Error: Missing output variables for print command.");
//         }
//
//         const AnalysisParams* analysis_to_run = nullptr;
//         AnalysisParams temp_analysis;
//
//         if (!analysisParams.empty()){ // پارامترها در دستور پرینت آمده
//             temp_analysis.type = analysisType;
//             temp_analysis.params = analysisParams;
//             analysis_to_run = &temp_analysis;
//         } else if (last_analysis.type == analysisType) { // استفاده از آخرین تحلیل تعریف شده
//             analysis_to_run = &last_analysis;
//         } else {
//             throw logic_error("Error: No analysis parameters provided or defined for " + analysisType);
//         }
//
//         // آماده‌سازی مدار برای تحلیل
//         vector<Node*> nodes_vector;
//         for(auto const& [name, ptr] : nodes_map) nodes_vector.push_back(ptr);
//         circuit_manager.setCircuitData(nodes_vector, elements_list);
//
//
//         if (analysis_to_run->type == "TRAN") {
//             if (analysis_to_run->params.size() < 2) throw logic_error("Error: Not enough parameters for TRAN analysis.");
//             double tstep = stringToDouble(analysis_to_run->params[0]);
//             double tstop = stringToDouble(analysis_to_run->params[1]);
//             TransientAnalysis tran(circuit_manager, tstep, tstop);
//             tran.run(output_vars);
//
//         } else if (analysis_to_run->type == "DC") {
//             if (analysis_to_run->params.size() < 4) throw logic_error("Error: Not enough parameters for DC analysis.");
//             string srcName = analysis_to_run->params[0];
//             double start = stringToDouble(analysis_to_run->params[1]);
//             double end = stringToDouble(analysis_to_run->params[2]);
//             double inc = stringToDouble(analysis_to_run->params[3]);
//             DCSweepAnalysis dc(circuit_manager, srcName, start, end, inc);
//             dc.run(output_vars);
//         }
//     }
// };
//
//
// // ============================================================================================
// // ||                  بخش پردازشگر دستورات (View)                                             ||
// // || این بخش شامل کلاس‌ پردازشگر ورودی و توابع کمکی مربوطه می‌باشد.                             ||
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
//     string s = s_in;
//     size_t i = 0;
//     if (s[0] == '-') {
//         if (s.length() == 1) return false;
//         i = 1;
//     }
//     bool digitFound = false;
//     bool dotFound = false;
//     for (; i < s.length(); ++i) {
//         if (isdigit(s[i])) {
//             digitFound = true;
//         } else if (s[i] == '.') {
//             if (dotFound) return false;
//             dotFound = true;
//         } else {
//             return false;
//         }
//     }
//     if (!digitFound) return false;
//     try { stod(s); } catch (const std::invalid_argument&) { return false; } catch (const std::out_of_range&) { return false; }
//     return true;
// }
//
// bool checkingNemadElmi(const string& s_in) {
//     if (s_in.empty()) return false;
//     string s = s_in;
//     std::transform(s.begin(), s.end(), s.begin(), ::tolower);
//     size_t e_pos = s.find('e');
//     if (e_pos == string::npos || e_pos == 0 || e_pos == s.length() - 1) return false;
//     string base_str = s.substr(0, e_pos);
//     string exp_str = s.substr(e_pos + 1);
//     if (base_str.empty() || exp_str.empty()) return false;
//     if (!checkDouble(base_str)) return false;
//     size_t i = 0;
//     if (exp_str[0] == '+' || exp_str[0] == '-') {
//         if (exp_str.length() == 1) return false;
//         i = 1;
//     }
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
//     void validateParameter(const string& param_name, const string& param_val_raw, const string& context_name, bool allow_zero, bool must_be_positive) {
//         pair<string, string> p_param = valuate(param_val_raw);
//         if (!checkDouble(p_param.first) && !checkingNemadElmi(p_param.first)) {
//             throw logic_error("Error: Invalid format for " + param_name + " in " + context_name + " (" + param_val_raw + ")");
//         }
//         double val = stod(p_param.first);
//         if (must_be_positive && val <= 0) {
//             throw logic_error("Error: Parameter " + param_name + " must be positive in " + context_name + " (" + param_val_raw + ")");
//         }
//         if (!allow_zero && val == 0) {
//              throw logic_error("Error: Parameter " + param_name + " cannot be zero in " + context_name + " (" + param_val_raw + ")");
//         }
//     }
//
//     bool tryParseElementCommands(const string& in) {
//         smatch matches;
//         regex add_res_pattern(R"(^add (R\w*) (\S+) (\S+) (.+?)$)");
//         if (regex_match(in, matches, add_res_pattern)) {
//             validateParameter("Resistance", matches[4].str(), "Resistor " + matches[1].str(), false, true);
//             controller.makingResistor(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
//         }
//
//         regex add_cap_pattern(R"(^add (C\w+) (\S+) (\S+) (.+?)$)");
//         if (regex_match(in, matches, add_cap_pattern)) {
//             validateParameter("Capacitance", matches[4].str(), "Capacitor " + matches[1].str(), false, true);
//             controller.makingCapacity(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
//         }
//
//         regex add_ind_pattern(R"(^add (L\w+) (\S+) (\S+) (.+?)$)");
//         if (regex_match(in, matches, add_ind_pattern)) {
//             validateParameter("Inductance", matches[4].str(), "Inductor " + matches[1].str(), false, true);
//             controller.makingInductor(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
//         }
//
//         regex add_diode_pattern(R"(^add (D\w+) (\S+) (\S+) (D|Z)$)");
//         if (regex_match(in, matches, add_diode_pattern)) {
//             controller.makingDiode(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
//         }
//
//         regex add_gnd_pattern(R"(^add GND (\S+)$)");
//         if (regex_match(in, matches, add_gnd_pattern)) {
//             controller.addingGround(matches[1].str()); return true;
//         }
//
//         regex del_elem_pattern(R"(^delete ([RCLD]\w+)$)");
//         if(regex_match(in, matches, del_elem_pattern)) {
//             controller.deletingElement(matches[1].str()); return true;
//         }
//
//         return false;
//     }
//
//     bool tryParseSourceCommands(const string& in) {
//         smatch matches;
//         regex add_v_generic_pattern(R"(^add (V\w+) (\S+) (\S+) (.+?)$)");
//         if (regex_match(in, matches, add_v_generic_pattern)) {
//             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_full_str = trim(matches[4].str());
//             regex vsin_params_pattern(R"(^SIN\s*\(\s*(\S+?)\s*,\s*(\S+?)\s*,\s*(\S+?)\s*\)$)");
//             if (regex_match(val_full_str, matches, vsin_params_pattern)) {
//                 validateParameter("Frequency", matches[3].str(), "SIN source " + name, false, true);
//                 controller.makingVoltageSourceSin(name, n1, n2, matches[1].str(), matches[2].str(), matches[3].str()); return true;
//             }
//             regex vpulse_params_pattern(R"(^PULSE\s*\(\s*(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s*\)$)");
//             if (regex_match(val_full_str, matches, vpulse_params_pattern)) {
//                 validateParameter("Period", matches[7].str(), "PULSE source " + name, false, true);
//                 controller.makingVoltageSourcePulse(name, n1, n2, matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str(), matches[6].str(), matches[7].str());
//                 return true;
//             }
//             validateParameter("Value", val_full_str, "DC source " + name, true, false);
//             controller.makingVoltageSourceDC(name, n1, n2, val_full_str); return true;
//         }
//
//         regex add_i_pattern(R"(^add (I\w+) (\S+) (\S+) (.+?)$)");
//         if(regex_match(in, matches, add_i_pattern)){
//             validateParameter("Value", matches[4].str(), "CurrentSource " + matches[1].str(), true, false);
//             controller.makingCurrentSourceDC(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
//         }
//
//         regex add_vcvs_pattern(R"(^add (E\w+) (\S+) (\S+) (\S+) (\S+) (.+?)$)");
//         if (regex_match(in, matches, add_vcvs_pattern)) {
//              controller.makingVCVS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str(), matches[6].str()); return true;
//         }
//         regex add_vccs_pattern(R"(^add (G\w+) (\S+) (\S+) (\S+) (\S+) (.+?)$)");
//         if (regex_match(in, matches, add_vccs_pattern)) {
//             controller.makingVCCS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str(), matches[6].str()); return true;
//         }
//         regex add_ccvs_pattern(R"(^add (H\w+) (\S+) (\S+) (V\w+) (.+?)$)");
//         if (regex_match(in, matches, add_ccvs_pattern)) {
//             controller.makingCCVS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str()); return true;
//         }
//         regex add_cccs_pattern(R"(^add (F\w+) (\S+) (\S+) (V\w+) (.+?)$)");
//         if (regex_match(in, matches, add_cccs_pattern)) {
//             controller.makingCCCS(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str()); return true;
//         }
//
//         regex del_src_pattern(R"(^delete ([VIEGHF]\w+)$)");
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
//             controller.defineAnalysis("TRAN", params);
//             return true;
//         }
//
//         regex dc_def_pattern(R"(^\.DC\s+([VI]\w*)\s+(\S+)\s+(\S+)\s+(\S+)\s*$)");
//         if (regex_match(in, matches, dc_def_pattern)) {
//             vector<string> params = {matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()};
//             controller.defineAnalysis("DC", params);
//             return true;
//         }
//         return false;
//     }
//
//     bool tryParseNodeCommands(const string& in) {
//         if (in == "nodes") { controller.handleNodesCommand(); return true; }
//
//         smatch matches;
//         regex rename_node_full_pattern(R"(^rename node (\S+) (\S+)$)");
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
//         regex list_comp_pattern(R"(^list ([RCLDVIEGHF])$)");
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
//             // بررسی می‌کنیم که آیا پارامترهای تحلیل در دستور پرینت آمده‌اند یا نه
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
//             for(size_t i = var_start_index; i < words.size(); ++i) {
//                 vars_ss << words[i] << " ";
//             }
//             outputVarsStr = vars_ss.str();
//
//             controller.handlePrintCommand(analysisType, analysisParams, outputVarsStr);
//             return true;
//         }
//         return false;
//     }
//
// public:
//     explicit CommandParser(centralController& ctrl) : controller(ctrl) {}
//
//     void processInput(const string& in) {
//         if (trim(in).empty()) return;
//
//         string trimmed_in = trim(in);
//
//         if (tryParseAnalysisDefinitionCommands(trimmed_in)) return;
//         if (tryParseNodeCommands(trimmed_in)) return;
//         if (tryParseListCommands(trimmed_in)) return;
//         if (tryParsePrintCommands(trimmed_in)) return;
//         if (tryParseElementCommands(trimmed_in)) return;
//         if (tryParseSourceCommands(trimmed_in)) return;
//
//         throw logic_error("Error: Unknown command or invalid syntax. -> " + in);
//     }
// };
//
// // ============================================================================================
// // ||                  تابع اصلی (Main Function)                                              ||
// // ============================================================================================
// int main() {
//     centralController controller_instance;
//     CommandParser parser(controller_instance);
//
//     cout << "Welcome to Circuit Simulator!" << endl;
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
//     return 0;
// }
//
// #include <iostream>
// #include <vector>
// #include <string>
// #include <stdexcept>
// #include <map>
// #include <cmath>
// #include <regex>
// #include <algorithm>
// #include <iomanip>
// #include <fstream>      // اضافه شده برای عملیات فایل
// #include <sstream>      // اضافه شده برای کار با رشته‌ها
// #include <filesystem>   // اضافه شده برای یافتن فایل‌ها (نیاز به C++17)
//
// #include "Eigen/Dense" // کتابخانه Eigen برای ماتریس‌ها
//
// #ifndef M_PI
// #define M_PI 3.14159265358979323846
// #endif
//
// using namespace std;
//
// // ==========================================================================================
// // ||                                    بخش مدل (Model)                                   ||
// // || این بخش شامل کلاس‌های مربوط به منطق اصلی مدار، المان‌ها و تحلیل‌گرها می‌باشد.         ||
// // ==========================================================================================
//
// /**
//  * @class Node
//  * @brief A class to represent a node in an electrical circuit.
//  *
//  * This class stores the name, current voltage, and voltage from the previous time step.
//  */
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
//     double getVoltage() const { return voltage; }
//     void setVoltage(double v) { voltage = v; }
//     double getPreviousVoltage() const { return previousVoltage; }
//     void setPreviousVoltage(double pv) { previousVoltage = pv; }
//     void updateVoltageForNextStep() { previousVoltage = voltage; }
//     bool isGround() const { return name == "0" || name == "GND" || name == "gnd"; }
// };
//
// /**
//  * @class Element
//  * @brief Base abstract class for all circuit elements.
//  *
//  * All other elements inherit from this class.
//  */
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
//     virtual string getValueAsString() const {return to_string(getValue()); } // For saving to file
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
//     string getType() const override { return "R"; } // Matches file format
//     double getValue() const override { return resistance; }
//     string getValueAsString() const override { return to_string(resistance); }
//     void setResistance(double res) { if (res > 0) this->resistance = res; }
//     double getCurrent() const override { return (node1->getVoltage() - node2->getVoltage()) / resistance; }
// };
//
// class VoltageSource : public Element {
// public:
//     enum SourceType { DC, SIN, PULSE };
// private:
//     double value;
//     double currentThroughSource;
//     SourceType sourceType;
//     // Parameters for sinusoidal and pulse sources
//     double dcOffset, amplitude, frequency;
//     double initialValue, pulsedValue, delayTime, riseTime, fallTime, onTime, period;
// public:
//     VoltageSource(Node* n1, Node* n2, const string &name, double dc_val)
//             : Element(n1, n2, name), value(dc_val), currentThroughSource(0.0), sourceType(DC) {}
//
//     void setSinParams(double offset, double amp, double freq) {
//         sourceType = SIN;
//         dcOffset = offset;
//         amplitude = amp;
//         frequency = freq > 0 ? freq : 0;
//     }
//
//     void setPulseParams(double v1, double v2, double td, double tr, double tf, double ton, double tper) {
//         sourceType = PULSE;
//         initialValue = v1;
//         pulsedValue = v2;
//         delayTime = td >= 0 ? td : 0;
//         riseTime = tr > 0 ? tr : 1e-9;
//         fallTime = tf > 0 ? tf : 1e-9;
//         onTime = ton >= 0 ? ton : 0;
//         if (tper <= 0) throw std::invalid_argument("Pulse period for " + name + " must be positive.");
//         period = tper;
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
//                 value = pulsedValue - (pulsedValue - initialValue) * ((timeInCycle - riseTime - onTime) / fallTime);
//             } else {
//                 value = initialValue;
//             }
//         }
//     }
//
//     void setVoltageValue(double val) {
//         sourceType = DC;
//         value = val;
//     }
//     string getType() const override { return "V"; } // Matches file format
//     double getValue() const override { return value; }
//     string getValueAsString() const override {
//         if (sourceType == DC) return to_string(value);
//         if (sourceType == SIN) {
//             stringstream ss;
//             ss << "SIN(" << dcOffset << "," << amplitude << "," << frequency << ")";
//             return ss.str();
//         }
//         if (sourceType == PULSE){
//              stringstream ss;
//              ss << "PULSE(" << initialValue << " " << pulsedValue << " " << delayTime << " " << riseTime << " " << fallTime << " " << onTime << " " << period << ")";
//              return ss.str();
//         }
//         return to_string(value);
//     }
//     double getCurrent() const override { return currentThroughSource; }
//     void setCurrent(double current) override { this->currentThroughSource = current; }
// };
//
// class CurrentSource : public Element {
// private: double currentValue;
// public:
//     CurrentSource(Node* n1, Node* n2, const string &name, double val) : Element(n1, n2, name), currentValue(val) {}
//     string getType() const override { return "I"; } // Matches file format
//     double getValue() const override { return currentValue; }
//     string getValueAsString() const override { return to_string(currentValue); }
// };
//
// class Capacitor : public Element {
// private: double capacitance;
// public:
//     Capacitor(Node* n1, Node* n2, const string &name, double cap) : Element(n1, n2, name) {
//         if (cap <= 0) throw std::invalid_argument("Capacitance must be positive for " + name);
//         this->capacitance = cap;
//     }
//     string getType() const override { return "C"; } // Matches file format
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
//     string getType() const override { return "L"; } // Matches file format
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
//     string getType() const override { return "D"; } // Matches file format
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
//     string getType() const override { return "E"; } // Matches file format
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
//     string getType() const override { return "G"; } // Matches file format
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
//     string getType() const override { return "H"; } // Matches file format
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
//     string getType() const override { return "F"; } // Matches file format
//     double getValue() const override { return gain; }
//     string getValueAsString() const override {
//         stringstream ss;
//         ss << controlVoltageSourceName << " " << to_string(gain);
//         return ss.str();
//     }
//     string getControlVoltageSourceName() const { return controlVoltageSourceName; }
// };
//
// /**
//  * @class MakingMNA
//  * @brief Class to manage the circuit, build MNA matrices, and vector Z.
//  */
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
//         if (!groundNodeRef) throw std::runtime_error("Error: Ground node not detected.");
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
//     double getTimeStep() const { return this->timeStep_h; }
//     Element* getElement(const string& name) {
//         for (auto* elem : elementsInCircuit) if (elem->getName() == name) return elem;
//         return nullptr;
//     }
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
//                 if (!n1->isGround()) A(nodeToIndexMap[n1], nodeToIndexMap[n1]) += g;
//                 if (!n2->isGround()) A(nodeToIndexMap[n2], nodeToIndexMap[n2]) += g;
//                 if (!n1->isGround() && !n2->isGround()) {
//                     A(nodeToIndexMap[n1], nodeToIndexMap[n2]) -= g;
//                     A(nodeToIndexMap[n2], nodeToIndexMap[n1]) -= g;
//                 }
//             }
//             else if (auto cap = dynamic_cast<Capacitor*>(elem)) {
//                 if (!isDCAnalysis) {
//                     if (this->timeStep_h <= 0) throw std::runtime_error("Time step h is not set for capacitor " + cap->getName());
//                     double g = cap->getValue() / this->timeStep_h;
//                     if (!n1->isGround()) A(nodeToIndexMap[n1], nodeToIndexMap[n1]) += g;
//                     if (!n2->isGround()) A(nodeToIndexMap[n2], nodeToIndexMap[n2]) += g;
//                     if (!n1->isGround() && !n2->isGround()) {
//                         A(nodeToIndexMap[n1], nodeToIndexMap[n2]) -= g;
//                         A(nodeToIndexMap[n2], nodeToIndexMap[n1]) -= g;
//                     }
//                 }
//             }
//             else if (auto vs = dynamic_cast<VoltageSource*>(elem)) {
//                 int i = numNonGroundNodes + vsToIndexMap[vs];
//                 if (!n1->isGround()) { A(nodeToIndexMap[n1], i) += 1.0; A(i, nodeToIndexMap[n1]) += 1.0; }
//                 if (!n2->isGround()) { A(nodeToIndexMap[n2], i) -= 1.0; A(i, nodeToIndexMap[n2]) -= 1.0; }
//             }
//             else if (auto ind = dynamic_cast<Inductor*>(elem)) {
//                 int i = numNonGroundNodes + numVS + inductorToIndexMap[ind];
//                 if (isDCAnalysis) { // In DC analysis, inductor is a short circuit
//                     if (!n1->isGround()) A(i, nodeToIndexMap[n1]) += 1.0;
//                     if (!n2->isGround()) A(i, nodeToIndexMap[n2]) -= 1.0;
//                 } else { // In transient analysis
//                     double l_div_h = ind->getValue() / this->timeStep_h;
//                     if (!n1->isGround()) { A(nodeToIndexMap[n1], i) += 1.0; A(i, nodeToIndexMap[n1]) += 1.0; }
//                     if (!n2->isGround()) { A(nodeToIndexMap[n2], i) -= 1.0; A(i, nodeToIndexMap[n2]) -= 1.0; }
//                     A(i, i) -= l_div_h;
//                 }
//             }
//             else if (auto diode = dynamic_cast<IdealDiode*>(elem)) {
//                 int i = numNonGroundNodes + numVS + numL + idealDiodeToIndexMap[diode];
//                 if (diode->getState() == IdealDiode::ON) {
//                     if (!n1->isGround()) { A(nodeToIndexMap[n1], i) += 1.0; A(i, nodeToIndexMap[n1]) += 1.0; }
//                     if (!n2->isGround()) { A(nodeToIndexMap[n2], i) -= 1.0; A(i, nodeToIndexMap[n2]) -= 1.0; }
//                 } else { // OFF State
//                     A(i, i) = 1.0;
//                 }
//             }
//             else if (auto vccs = dynamic_cast<VCCS*>(elem)) {
//                 double g = vccs->getValue();
//                 Node* cn1 = vccs->getControlNode1(); Node* cn2 = vccs->getControlNode2();
//                 if (!n1->isGround() && !cn1->isGround()) A(nodeToIndexMap[n1], nodeToIndexMap[cn1]) += g;
//                 if (!n1->isGround() && !cn2->isGround()) A(nodeToIndexMap[n1], nodeToIndexMap[cn2]) -= g;
//                 if (!n2->isGround() && !cn1->isGround()) A(nodeToIndexMap[n2], nodeToIndexMap[cn1]) -= g;
//                 if (!n2->isGround() && !cn2->isGround()) A(nodeToIndexMap[n2], nodeToIndexMap[cn2]) += g;
//             }
//             else if (auto cccs = dynamic_cast<CCCS*>(elem)) {
//                 double gain = cccs->getValue();
//                 VoltageSource* ctrl_vs = nullptr;
//                 for(auto* vs_ptr : orderedVoltageSources) if(vs_ptr->getName() == cccs->getControlVoltageSourceName()) ctrl_vs = vs_ptr;
//                 if (!ctrl_vs) throw std::runtime_error("Error: Dependent source '" + cccs->getName() + "' has an undefined control element '" + cccs->getControlVoltageSourceName() + "'.");
//                 int ctrl_i = numNonGroundNodes + vsToIndexMap[ctrl_vs];
//                 if (!n1->isGround()) A(nodeToIndexMap[n1], ctrl_i) += gain;
//                 if (!n2->isGround()) A(nodeToIndexMap[n2], ctrl_i) -= gain;
//             }
//             else if (auto vcvs = dynamic_cast<VCVS*>(elem)) {
//                 int i = numNonGroundNodes + numVS + numL + numD + vcvsToIndexMap[vcvs];
//                 Node* cn1 = vcvs->getControlNode1(); Node* cn2 = vcvs->getControlNode2();
//                 double gain = vcvs->getValue();
//                 if (!n1->isGround()) { A(nodeToIndexMap[n1], i) += 1.0; A(i, nodeToIndexMap[n1]) += 1.0; }
//                 if (!n2->isGround()) { A(nodeToIndexMap[n2], i) -= 1.0; A(i, nodeToIndexMap[n2]) -= 1.0; }
//                 if (!cn1->isGround()) A(i, nodeToIndexMap[cn1]) -= gain;
//                 if (!cn2->isGround()) A(i, nodeToIndexMap[cn2]) += gain;
//             }
//             else if (auto ccvs = dynamic_cast<CCVS*>(elem)) {
//                 int i = numNonGroundNodes + numVS + numL + numD + numVCVS + ccvsToIndexMap[ccvs];
//                 double gain = ccvs->getValue();
//                 VoltageSource* ctrl_vs = nullptr;
//                 for(auto* vs_ptr : orderedVoltageSources) if(vs_ptr->getName() == ccvs->getControlVoltageSourceName()) ctrl_vs = vs_ptr;
//                 if (!ctrl_vs) throw std::runtime_error("Error: Dependent source '" + ccvs->getName() + "' has an undefined control element '" + ccvs->getControlVoltageSourceName() + "'.");
//                 int ctrl_i = numNonGroundNodes + vsToIndexMap[ctrl_vs];
//                 if (!n1->isGround()) {A(i, nodeToIndexMap[n1]) += 1.0;} else {A(i, nodeToIndexMap[n1])=0;};
//                 if (!n2->isGround()) {A(i, nodeToIndexMap[n2]) -= 1.0;} else {A(i, nodeToIndexMap[n2])=0;};
//                 A(i, ctrl_i) -= gain;
//             }
//         }
//         return A;
//     }
//
//     Eigen::VectorXd getSystemVectorZ(bool isDCAnalysis = false) {
//         int systemSize = orderedNonGroundNodes.size() + orderedVoltageSources.size() + orderedInductors.size() + orderedIdealDiodes.size() + orderedVCVS.size() + orderedCCVS.size();
//         if (systemSize == 0) return Eigen::VectorXd(0);
//         Eigen::VectorXd Z = Eigen::VectorXd::Zero(systemSize);
//
//         for (Element* elem : elementsInCircuit) {
//             if (auto cs = dynamic_cast<CurrentSource*>(elem)) {
//                 if (!cs->getNode1()->isGround()) Z(nodeToIndexMap[cs->getNode1()]) -= cs->getValue();
//                 if (!cs->getNode2()->isGround()) Z(nodeToIndexMap[cs->getNode2()]) += cs->getValue();
//             } else if (auto cap = dynamic_cast<Capacitor*>(elem)) {
//                 if (!isDCAnalysis) {
//                     if (this->timeStep_h <= 0) throw std::runtime_error("Time step h is not set for capacitor " + cap->getName());
//                     double c_div_h = cap->getValue() / this->timeStep_h;
//                     double v_n1_prev = cap->getNode1()->getPreviousVoltage();
//                     double v_n2_prev = cap->getNode2()->getPreviousVoltage();
//                     double i_eq_cap = c_div_h * (v_n1_prev - v_n2_prev);
//                     if (!cap->getNode1()->isGround()) Z(nodeToIndexMap[cap->getNode1()]) += i_eq_cap;
//                     if (!cap->getNode2()->isGround()) Z(nodeToIndexMap[cap->getNode2()]) -= i_eq_cap;
//                 }
//             }
//         }
//
//         for (const auto& vs : orderedVoltageSources) Z(orderedNonGroundNodes.size() + vsToIndexMap[vs]) = vs->getValue();
//         for (const auto& ind : orderedInductors) {
//             int i = orderedNonGroundNodes.size() + orderedVoltageSources.size() + inductorToIndexMap[ind];
//             if (!isDCAnalysis) {
//                 Z(i) = - (ind->getValue() / this->timeStep_h) * ind->getPreviousCurrent();
//             }
//         }
//         for (const auto& diode : orderedIdealDiodes) {
//             int i = orderedNonGroundNodes.size() + orderedVoltageSources.size() + orderedInductors.size() + idealDiodeToIndexMap[diode];
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
//         if (A.rows() > 0 && std::abs(lu.determinant()) < 1e-14) {
//              throw std::runtime_error("Error: System matrix is singular or ill-conditioned. The circuit may not be solvable.");
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
// class TransientAnalysis {
// private:
//     MakingMNA& mnaCircuit;
//     MNASolver solver;
//     double t_step, t_stop;
// public:
//     TransientAnalysis(MakingMNA& circuit, double step, double stop)
//             : mnaCircuit(circuit), solver(), t_step(step), t_stop(stop) {
//         if (t_step <= 0 || t_stop <= 0 || t_step > t_stop) {
//             throw std::invalid_argument("Invalid time parameters for transient analysis.");
//         }
//     }
//
//     void run(const vector<string>& output_vars) {
//         double current_time = 0.0;
//
//         cout << "Time(s)\t";
//         for (const auto& var : output_vars) cout << var << "\t";
//         cout << endl;
//         cout << "----------------------------------------------------------------" << endl;
//
//         mnaCircuit.setTimeStep(t_step);
//
//         while (current_time <= t_stop) {
//             for (auto* elem : mnaCircuit.getAllElements()) {
//                 elem->updateValue(current_time);
//             }
//
//             bool diodes_converged = false;
//             int max_diode_iterations = 25;
//             int iteration_count = 0;
//             while (!diodes_converged && iteration_count < max_diode_iterations) {
//                  iteration_count++;
//                 diodes_converged = true;
//
//                 Eigen::MatrixXd A = mnaCircuit.getSystemMatrixA();
//                 Eigen::VectorXd Z = mnaCircuit.getSystemVectorZ();
//                 Eigen::VectorXd X = solver.solve(A, Z);
//                 solver.updateCircuitState(X, mnaCircuit);
//
//                 for (auto* diode_ptr : mnaCircuit.getOrderedIdealDiodes()) {
//                     IdealDiode::State old_state = diode_ptr->getState();
//                     double v_anode = diode_ptr->getNode1()->getVoltage();
//                     double v_cathode = diode_ptr->getNode2()->getVoltage();
//                     if (old_state == IdealDiode::OFF) {
//                         if (v_anode > v_cathode + diode_ptr->getForwardVoltage()) {
//                             diode_ptr->setState(IdealDiode::ON);
//                             diodes_converged = false;
//                         }
//                     } else { // ON State
//                         if (diode_ptr->getCurrent() < 0) {
//                             diode_ptr->setState(IdealDiode::OFF);
//                             diodes_converged = false;
//                         }
//                     }
//                 }
//             }
//             if (!diodes_converged) cout << "Warning: Diode states failed to converge at t=" << current_time << endl;
//
//             // Print results
//             cout << fixed << setprecision(6) << current_time << "\t";
//             for (const auto& var : output_vars) {
//                  if (var[0] == 'V' && var[1] == '(') {
//                     string node_name = var.substr(2, var.length() - 3);
//                     bool found = false;
//                     for (auto* node : mnaCircuit.getAllNodesInCircuit()) {
//                         if (node->getName() == node_name) {
//                             cout << node->getVoltage() << "\t";
//                             found = true;
//                             break;
//                         }
//                     }
//                     if (!found) cout << "NaN\t";
//                 } else if (var[0] == 'I' && var[1] == '(') {
//                     string elem_name = var.substr(2, var.length() - 3);
//                     Element* elem = mnaCircuit.getElement(elem_name);
//                     if (elem) {
//                         cout << elem->getCurrent() << "\t";
//                     } else {
//                         cout << "NaN\t";
//                     }
//                 }
//             }
//             cout << endl;
//
//             current_time += t_step;
//
//             for (auto* node : mnaCircuit.getAllNodesInCircuit()) node->updateVoltageForNextStep();
//             for (auto* ind : mnaCircuit.getOrderedInductors()) ind->updateCurrentForNextStep();
//         }
//     }
// };
//
// class DCSweepAnalysis {
// private:
//     MakingMNA& mnaCircuit;
//     MNASolver solver;
//     string sweepComponentName;
//     double startValue, endValue, increment;
// public:
//     DCSweepAnalysis(MakingMNA& circuit, const string& compName, double start, double end, double inc)
//             : mnaCircuit(circuit), solver(), sweepComponentName(compName), startValue(start), endValue(end), increment(inc) {
//         if (increment == 0) throw std::invalid_argument("DC sweep increment cannot be zero.");
//         if ((endValue > startValue && increment < 0) || (endValue < startValue && increment > 0)) {
//             increment = -increment;
//         }
//     }
//     void run(const vector<string>& output_vars) {
//         Element* sweepElement = mnaCircuit.getElement(sweepComponentName);
//         if (!sweepElement) throw std::runtime_error("Sweep component '" + sweepComponentName + "' not found.");
//         auto* sweepVoltageSource = dynamic_cast<VoltageSource*>(sweepElement);
//         if (!sweepVoltageSource) throw std::runtime_error("Sweep component must be a VoltageSource.");
//
//         cout << sweepComponentName << "\t";
//         for (const auto& var : output_vars) cout << var << "\t";
//         cout << endl;
//         cout << "-----------------------------------------------------" << endl;
//
//         for (double val = startValue; (increment > 0 ? val <= endValue : val >= endValue) ; val += increment) {
//             sweepVoltageSource->setVoltageValue(val);
//
//             Eigen::MatrixXd A = mnaCircuit.getSystemMatrixA(true);
//             Eigen::VectorXd Z = mnaCircuit.getSystemVectorZ(true);
//             Eigen::VectorXd X = solver.solve(A, Z);
//             solver.updateCircuitState(X, mnaCircuit);
//
//             // Print results
//             cout << fixed << setprecision(6) << val << "\t";
//             for (const auto& var : output_vars) {
//                  if (var[0] == 'V' && var[1] == '(') {
//                     string node_name = var.substr(2, var.length() - 3);
//                     bool found = false;
//                     for (auto* node : mnaCircuit.getAllNodesInCircuit()) {
//                         if (node->getName() == node_name) {
//                             cout << node->getVoltage() << "\t";
//                             found = true;
//                             break;
//                         }
//                     }
//                     if (!found) cout << "NaN\t";
//                 } else if (var[0] == 'I' && var[1] == '(') {
//                     string elem_name = var.substr(2, var.length() - 3);
//                     Element* elem = mnaCircuit.getElement(elem_name);
//                     if (elem) {
//                         cout << elem->getCurrent() << "\t";
//                     } else {
//                         cout << "NaN\t";
//                     }
//                 }
//             }
//             cout << endl;
//         }
//     }
// };
//
// // ============================================================================================
// // ||                  بخش کنترلر و نمایش (Controller & View)                                 ||
// // ||         >>> این بخش برای افزودن قابلیت‌های مدیریت فایل اصلاح شده است <<<                 ||
// // ============================================================================================
//
// // Forward declaration
// class CommandParser;
//
// // Helper functions for input processing
// bool checkDouble(const string& s);
// bool checkingNemadElmi(const string& s);
// pair<string, string> valuate(const string& s);
// double stringToDouble(const string& s);
// string trim(const string& str);
//
// // Struct to hold analysis parameters
// struct AnalysisParams {
//     string type; // "TRAN" or "DC"
//     vector<string> params;
// };
//
// /**
//  * @class centralController
//  * @brief Central controller class to manage all simulator operations.
//  */
// class centralController {
// private:
//     MakingMNA circuit_manager;
//     map<string, Node*> nodes_map;
//     vector<Element*> elements_list;
//     map<string, Element*> elements_map;
//     AnalysisParams last_analysis;
//
//     // Added variables for file management
//     bool is_in_file_menu = false;
//     vector<string> schematic_files;
//     string schematics_directory = "./circuits";
//
//     // Helper method to get or create a node
//     Node* getNode(const string& name) {
//         if (nodes_map.find(name) == nodes_map.end()) {
//             nodes_map[name] = new Node(name);
//         }
//         return nodes_map[name];
//     }
//
//     // Method to clear the current circuit from memory
//     void clearCircuit() {
//         for (auto const& [name, node_ptr] : nodes_map) {
//             delete node_ptr;
//         }
//         for (auto elem_ptr : elements_list) {
//             delete elem_ptr;
//         }
//         nodes_map.clear();
//         elements_list.clear();
//         elements_map.clear();
//         cout << "Current circuit cleared." << endl;
//     }
//
//     // Method to load available schematic files
//     void loadAvailableSchematics() {
//         schematic_files.clear();
//         try {
//             if (!filesystem::exists(schematics_directory)) {
//                 filesystem::create_directory(schematics_directory);
//             }
//             for (const auto & entry : filesystem::directory_iterator(schematics_directory)) {
//                 if (entry.is_regular_file() && entry.path().extension() == ".txt") {
//                     schematic_files.push_back(entry.path().stem().string());
//                 }
//             }
//             sort(schematic_files.begin(), schematic_files.end());
//         } catch (const filesystem::filesystem_error& e) {
//             cerr << "Filesystem error: " << e.what() << endl;
//         }
//     }
//
//
// public:
//     centralController() {
//         loadAvailableSchematics(); // Load existing files on startup
//     }
//
//     ~centralController() {
//         clearCircuit();
//     }
//
//     // --- Added methods for file menu ---
//
//     bool isInFileMenu() const { return is_in_file_menu; }
//
//     void handleShowExistingSchematicsCommand() {
//         is_in_file_menu = true;
//         loadAvailableSchematics(); // Ensure the list is up-to-date
//
//         cout << "-choose existing schematic:" << endl;
//         if (schematic_files.empty()) {
//             cout << " (No schematics found in '" << schematics_directory << "' directory)" << endl;
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
//
//         if (!file.is_open()) {
//             cout << "Error: Could not create file " << full_path << endl;
//             return;
//         }
//
//         // Save all elements to the file
//         for (const auto* elem : elements_list) {
//              file << elem->getName() << " "
//                  << elem->getNode1()->getName() << " "
//                  << elem->getNode2()->getName() << " "
//                  << elem->getValueAsString() << endl;
//         }
//
//         file.close();
//         cout << "SUCCESS: Current circuit saved to '" << filename << ".txt'." << endl;
//
//         // If in the menu, refresh the menu to show the new file
//         if (is_in_file_menu) {
//             handleShowExistingSchematicsCommand();
//         }
//     }
//
//
//     // --- Element Management ---
//     void makingResistor(const string& name, const string& n1_name, const string& n2_name, const string& val_str) {
//         if (elements_map.count(name)) throw logic_error("Error: Resistor " + name + " already exists in the circuit");
//         double resistance = stringToDouble(val_str);
//         Node* n1 = getNode(n1_name);
//         Node* n2 = getNode(n2_name);
//         Resistor* res = new Resistor(n1, n2, name, resistance);
//         elements_list.push_back(res);
//         elements_map[name] = res;
//         cout << "SUCCESS: Resistor " << name << " added." << endl;
//     }
//
//     void makingCapacity(const string& name, const string& n1_name, const string& n2_name, const string& val_str) {
//         if (elements_map.count(name)) throw logic_error("Error: Capacitor " + name + " already exists in the circuit");
//         double capacitance = stringToDouble(val_str);
//         Node* n1 = getNode(n1_name);
//         Node* n2 = getNode(n2_name);
//         Capacitor* cap = new Capacitor(n1, n2, name, capacitance);
//         elements_list.push_back(cap);
//         elements_map[name] = cap;
//         cout << "SUCCESS: Capacitor " << name << " added." << endl;
//     }
//
//     void makingInductor(const string& name, const string& n1_name, const string& n2_name, const string& val_str) {
//         if (elements_map.count(name)) throw logic_error("Error: Inductor " + name + " already exists in the circuit");
//         double inductance = stringToDouble(val_str);
//         Node* n1 = getNode(n1_name);
//         Node* n2 = getNode(n2_name);
//         Inductor* ind = new Inductor(n1, n2, name, inductance);
//         elements_list.push_back(ind);
//         elements_map[name] = ind;
//         cout << "SUCCESS: Inductor " << name << " added." << endl;
//     }
//
//     void makingDiode(const string& name, const string& n1_name, const string& n2_name, const string& model) {
//         if (elements_map.count(name)) throw logic_error("Error: Diode " + name + " already exists in the circuit");
//         Node* n1 = getNode(n1_name);
//         Node* n2 = getNode(n2_name);
//         IdealDiode* diode = new IdealDiode(n1, n2, name, model);
//         elements_list.push_back(diode);
//         elements_map[name] = diode;
//         cout << "SUCCESS: Diode " << name << " with model " << model << " added." << endl;
//     }
//
//     // --- Source Management ---
//     void makingVoltageSourceDC(const string& name, const string& n_plus, const string& n_minus, const string& val_str) {
//         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
//         double voltage = stringToDouble(val_str);
//         Node* n1 = getNode(n_plus);
//         Node* n2 = getNode(n_minus);
//         VoltageSource* vs = new VoltageSource(n1, n2, name, voltage);
//         elements_list.push_back(vs);
//         elements_map[name] = vs;
//         cout << "SUCCESS: DC VoltageSource " << name << " added." << endl;
//     }
//
//     void makingCurrentSourceDC(const string& name, const string& n_plus, const string& n_minus, const string& val_str) {
//         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
//         double current = stringToDouble(val_str);
//         Node* n1 = getNode(n_plus);
//         Node* n2 = getNode(n_minus);
//         CurrentSource* cs = new CurrentSource(n1, n2, name, current);
//         elements_list.push_back(cs);
//         elements_map[name] = cs;
//         cout << "SUCCESS: DC CurrentSource " << name << " added." << endl;
//     }
//
//     void makingVoltageSourceSin(const string& name, const string& n_plus, const string& n_minus, const string& v_off, const string& v_amp, const string& freq) {
//          if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
//         Node* n1 = getNode(n_plus);
//         Node* n2 = getNode(n_minus);
//         VoltageSource* vs = new VoltageSource(n1, n2, name, 0.0);
//         vs->setSinParams(stringToDouble(v_off), stringToDouble(v_amp), stringToDouble(freq));
//         elements_list.push_back(vs);
//         elements_map[name] = vs;
//         cout << "SUCCESS: Sinusoidal Voltage Source " << name << " added." << endl;
//     }
//
//     void makingVoltageSourcePulse(const string& name, const string& n_plus, const string& n_minus,
//                                   const string& v1, const string& v2, const string& td,
//                                   const string& tr, const string& tf, const string& pw,
//                                   const string& period) {
//         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
//         Node* n1 = getNode(n_plus);
//         Node* n2 = getNode(n_minus);
//         VoltageSource* vs = new VoltageSource(n1, n2, name, 0.0);
//         vs->setPulseParams(stringToDouble(v1), stringToDouble(v2), stringToDouble(td), stringToDouble(tr), stringToDouble(tf), stringToDouble(pw), stringToDouble(period));
//         elements_list.push_back(vs);
//         elements_map[name] = vs;
//         cout << "SUCCESS: PULSE Voltage Source " << name << " added." << endl;
//     }
//
//     void makingVCVS(const string& name, const string& n_p, const string& n_m, const string& nc_p, const string& nc_m, const string& gain_str) {
//         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
//         double gain = stringToDouble(gain_str);
//         VCVS* vcvs = new VCVS(getNode(n_p), getNode(n_m), name, getNode(nc_p), getNode(nc_m), gain);
//         elements_list.push_back(vcvs);
//         elements_map[name] = vcvs;
//         cout << "SUCCESS: VCVS " << name << " added." << endl;
//     }
//
//     void makingVCCS(const string& name, const string& n_p, const string& n_m, const string& nc_p, const string& nc_m, const string& gain_str) {
//          if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
//         double gain = stringToDouble(gain_str);
//         VCCS* vccs = new VCCS(getNode(n_p), getNode(n_m), name, getNode(nc_p), getNode(nc_m), gain);
//         elements_list.push_back(vccs);
//         elements_map[name] = vccs;
//         cout << "SUCCESS: VCCS " << name << " added." << endl;
//     }
//
//     void makingCCVS(const string& name, const string& n_p, const string& n_m, const string& vctrl_name, const string& gain_str) {
//         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
//         if (!elements_map.count(vctrl_name) || dynamic_cast<VoltageSource*>(elements_map[vctrl_name]) == nullptr) {
//             throw logic_error("Error: Control voltage source " + vctrl_name + " not found for CCVS " + name);
//         }
//         double gain = stringToDouble(gain_str);
//         CCVS* ccvs = new CCVS(getNode(n_p), getNode(n_m), name, vctrl_name, gain);
//         elements_list.push_back(ccvs);
//         elements_map[name] = ccvs;
//         cout << "SUCCESS: CCVS " << name << " added." << endl;
//     }
//
//     void makingCCCS(const string& name, const string& n_p, const string& n_m, const string& vctrl_name, const string& gain_str) {
//         if (elements_map.count(name)) throw logic_error("Error: Source " + name + " already exists in the circuit");
//         if (!elements_map.count(vctrl_name) || dynamic_cast<VoltageSource*>(elements_map[vctrl_name]) == nullptr) {
//             throw logic_error("Error: Control voltage source " + vctrl_name + " not found for CCCS " + name);
//         }
//         double gain = stringToDouble(gain_str);
//         CCCS* cccs = new CCCS(getNode(n_p), getNode(n_m), name, vctrl_name, gain);
//         elements_list.push_back(cccs);
//         elements_map[name] = cccs;
//         cout << "SUCCESS: CCCS " << name << " added." << endl;
//     }
//
//     void deletingElement(const string& name) {
//         if (!elements_map.count(name)) throw logic_error("Error: Cannot delete component; component not found");
//
//         Element* elem_to_delete = elements_map[name];
//
//         // Remove from list and map
//         elements_map.erase(name);
//         elements_list.erase(remove(elements_list.begin(), elements_list.end(), elem_to_delete), elements_list.end());
//
//         delete elem_to_delete;
//         cout << "SUCCESS: Component " << name << " deleted." << endl;
//     }
//
//     void addingGround(const string& node_name){
//         // In our model, a node named "0" or "GND" is automatically ground.
//         // This function can ensure the ground node exists.
//         getNode(node_name); // Ensures the node exists.
//         cout << "SUCCESS: Node " << node_name << " is designated as a ground reference." << endl;
//     }
//
//     // --- Node and List Management ---
//     void handleNodesCommand() {
//         if (nodes_map.empty()) {
//             cout << "No nodes in the circuit yet." << endl;
//             return;
//         }
//         cout << "Available nodes:" << endl;
//         for(auto const& [name, node_ptr] : nodes_map) {
//             cout << name << " ";
//         }
//         cout << endl;
//     }
//
//     void handleListCommand(const string& componentType = "") {
//         if (elements_list.empty()) {
//             cout << "No elements in the circuit yet." << endl;
//             return;
//         }
//         bool found = false;
//         cout << "Circuit elements:" << endl;
//         for(auto const& elem : elements_list) {
//             bool type_match = false;
//             string elem_type_prefix = elem->getName().substr(0,1);
//             if (componentType.empty()) type_match = true;
//             else if (componentType == elem_type_prefix) type_match = true;
//
//             if (type_match) {
//                 cout << "- " << elem->getName() << " connected between "
//                      << elem->getNode1()->getName() << " and " << elem->getNode2()->getName() << endl;
//                 found = true;
//             }
//         }
//         if (!found) {
//             cout << "No elements of type '" << componentType << "' found." << endl;
//         }
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
//         nodes_map.erase(oldName);
//         nodes_map[newName] = node_ptr;
//         cout << "SUCCESS: Node renamed from " << oldName << " to " << newName << endl;
//     }
//
//     // --- Analysis Management and Execution ---
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
//         while (ss >> temp) {
//             output_vars.push_back(temp);
//         }
//
//         if (output_vars.empty()){
//             throw logic_error("Error: Missing output variables for print command.");
//         }
//
//         const AnalysisParams* analysis_to_run = nullptr;
//         AnalysisParams temp_analysis;
//
//         if (!analysisParams.empty()){ // Parameters provided in print command
//             temp_analysis.type = analysisType;
//             temp_analysis.params = analysisParams;
//             analysis_to_run = &temp_analysis;
//         } else if (last_analysis.type == analysisType) { // Use last defined analysis
//             analysis_to_run = &last_analysis;
//         } else {
//             throw logic_error("Error: No analysis parameters provided or defined for " + analysisType);
//         }
//
//         // Prepare circuit for analysis
//         vector<Node*> nodes_vector;
//         for(auto const& [name, ptr] : nodes_map) nodes_vector.push_back(ptr);
//         circuit_manager.setCircuitData(nodes_vector, elements_list);
//
//
//         if (analysis_to_run->type == "TRAN") {
//             if (analysis_to_run->params.size() < 2) throw logic_error("Error: Not enough parameters for TRAN analysis.");
//             double tstep = stringToDouble(analysis_to_run->params[0]);
//             double tstop = stringToDouble(analysis_to_run->params[1]);
//             TransientAnalysis tran(circuit_manager, tstep, tstop);
//             tran.run(output_vars);
//
//         } else if (analysis_to_run->type == "DC") {
//             if (analysis_to_run->params.size() < 4) throw logic_error("Error: Not enough parameters for DC analysis.");
//             string srcName = analysis_to_run->params[0];
//             double start = stringToDouble(analysis_to_run->params[1]);
//             double end = stringToDouble(analysis_to_run->params[2]);
//             double inc = stringToDouble(analysis_to_run->params[3]);
//             DCSweepAnalysis dc(circuit_manager, srcName, start, end, inc);
//             dc.run(output_vars);
//         }
//     }
// };
//
//
// // ============================================================================================
// // ||                  بخش پردازشگر دستورات (View)                                             ||
// // ||         >>> این بخش برای افزودن قابلیت‌های مدیریت فایل اصلاح شده است <<<                 ||
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
//     string s = s_in;
//     size_t i = 0;
//     if (s[0] == '-') {
//         if (s.length() == 1) return false;
//         i = 1;
//     }
//     bool digitFound = false;
//     bool dotFound = false;
//     for (; i < s.length(); ++i) {
//         if (isdigit(s[i])) {
//             digitFound = true;
//         } else if (s[i] == '.') {
//             if (dotFound) return false;
//             dotFound = true;
//         } else {
//             return false;
//         }
//     }
//     if (!digitFound) return false;
//     try { stod(s); } catch (const std::invalid_argument&) { return false; } catch (const std::out_of_range&) { return false; }
//     return true;
// }
//
// bool checkingNemadElmi(const string& s_in) {
//     if (s_in.empty()) return false;
//     string s = s_in;
//     std::transform(s.begin(), s.end(), s.begin(), ::tolower);
//     size_t e_pos = s.find('e');
//     if (e_pos == string::npos || e_pos == 0 || e_pos == s.length() - 1) return false;
//     string base_str = s.substr(0, e_pos);
//     string exp_str = s.substr(e_pos + 1);
//     if (base_str.empty() || exp_str.empty()) return false;
//     if (!checkDouble(base_str)) return false;
//     size_t i = 0;
//     if (exp_str[0] == '+' || exp_str[0] == '-') {
//         if (exp_str.length() == 1) return false;
//         i = 1;
//     }
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
//     void validateParameter(const string& param_name, const string& param_val_raw, const string& context_name, bool allow_zero, bool must_be_positive) {
//         pair<string, string> p_param = valuate(param_val_raw);
//         if (!checkDouble(p_param.first) && !checkingNemadElmi(p_param.first)) {
//             throw logic_error("Error: Invalid format for " + param_name + " in " + context_name + " (" + param_val_raw + ")");
//         }
//         double val = stod(p_param.first);
//         if (must_be_positive && val <= 0) {
//             throw logic_error("Error: Parameter " + param_name + " must be positive in " + context_name + " (" + param_val_raw + ")");
//         }
//         if (!allow_zero && val == 0) {
//              throw logic_error("Error: Parameter " + param_name + " cannot be zero in " + context_name + " (" + param_val_raw + ")");
//         }
//     }
//
//     // Parses SPICE-like netlist lines (e.g., from a file)
//     bool tryParseNetlistLine(const string& in) {
//         smatch matches;
//         // This regex is broad, captures <NAME> <NODE1> <NODE2> <...VALUES...>
//         regex netlist_pattern(R"(^([a-zA-Z]\S*)\s+(\S+)\s+(\S+)\s+(.+)$)");
//         if (regex_match(in, matches, netlist_pattern)) {
//             string name = matches[1].str();
//             char type = toupper(name[0]);
//             string n1 = matches[2].str();
//             string n2 = matches[3].str();
//             string value_part = matches[4].str();
//
//             // Reconstruct the "add" command to reuse existing parsers
//             stringstream ss;
//             ss << "add " << name << " " << n1 << " " << n2 << " " << value_part;
//             string command = ss.str();
//
//             // We must avoid calling processInput recursively to prevent infinite loops.
//             // So we call the specific parsers.
//             if (tryParseElementCommands(command) || tryParseSourceCommands(command)) {
//                 return true;
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
//             validateParameter("Resistance", matches[4].str(), "Resistor " + matches[1].str(), false, true);
//             controller.makingResistor(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
//         }
//
//         regex add_cap_pattern(R"(^add\s+(C\S*)\s+(\S+)\s+(\S+)\s+(.+)$)");
//         if (regex_match(in, matches, add_cap_pattern)) {
//             validateParameter("Capacitance", matches[4].str(), "Capacitor " + matches[1].str(), false, true);
//             controller.makingCapacity(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
//         }
//
//         regex add_ind_pattern(R"(^add\s+(L\S*)\s+(\S+)\s+(\S+)\s+(.+)$)");
//         if (regex_match(in, matches, add_ind_pattern)) {
//             validateParameter("Inductance", matches[4].str(), "Inductor " + matches[1].str(), false, true);
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
//         regex del_elem_pattern(R"(^delete\s+([RCLD]\S+)$)");
//         if(regex_match(in, matches, del_elem_pattern)) {
//             controller.deletingElement(matches[1].str()); return true;
//         }
//
//         return false;
//     }
//
//     bool tryParseSourceCommands(const string& in) {
//         smatch matches;
//         regex add_v_generic_pattern(R"(^add\s+(V\S*)\s+(\S+)\s+(\S+)\s+(.+)$)");
//         if (regex_match(in, matches, add_v_generic_pattern)) {
//             string name = matches[1].str(); string n1 = matches[2].str(); string n2 = matches[3].str(); string val_full_str = trim(matches[4].str());
//
//             regex vsin_params_pattern(R"(^SIN\s*\(\s*(\S+?)\s*,\s*(\S+?)\s*,\s*(\S+?)\s*\)$)");
//             smatch sin_matches;
//             if (regex_match(val_full_str, sin_matches, vsin_params_pattern)) {
//                 validateParameter("Frequency", sin_matches[3].str(), "SIN source " + name, false, true);
//                 controller.makingVoltageSourceSin(name, n1, n2, sin_matches[1].str(), sin_matches[2].str(), sin_matches[3].str()); return true;
//             }
//             regex vpulse_params_pattern(R"(^PULSE\s*\(\s*(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s*\)$)");
//             smatch pulse_matches;
//             if (regex_match(val_full_str, pulse_matches, vpulse_params_pattern)) {
//                 validateParameter("Period", pulse_matches[7].str(), "PULSE source " + name, false, true);
//                 controller.makingVoltageSourcePulse(name, n1, n2, pulse_matches[1].str(), pulse_matches[2].str(), pulse_matches[3].str(), pulse_matches[4].str(), pulse_matches[5].str(), pulse_matches[6].str(), pulse_matches[7].str());
//                 return true;
//             }
//             validateParameter("Value", val_full_str, "DC source " + name, true, false);
//             controller.makingVoltageSourceDC(name, n1, n2, val_full_str); return true;
//         }
//
//         regex add_i_pattern(R"(^add\s+(I\S*)\s+(\S+)\s+(\S+)\s+(.+)$)");
//         if(regex_match(in, matches, add_i_pattern)){
//             validateParameter("Value", matches[4].str(), "CurrentSource " + matches[1].str(), true, false);
//             controller.makingCurrentSourceDC(matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()); return true;
//         }
//
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
//
//         regex del_src_pattern(R"(^delete\s+([VIEGHF]\S+)$)");
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
//             controller.defineAnalysis("TRAN", params);
//             return true;
//         }
//
//         regex dc_def_pattern(R"(^\.DC\s+([VI]\w*)\s+(\S+)\s+(\S+)\s+(\S+)\s*$)");
//         if (regex_match(in, matches, dc_def_pattern)) {
//             vector<string> params = {matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()};
//             controller.defineAnalysis("DC", params);
//             return true;
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
//             // Check if analysis parameters are provided in the print command
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
//             for(size_t i = var_start_index; i < words.size(); ++i) {
//                 vars_ss << words[i] << " ";
//             }
//             outputVarsStr = vars_ss.str();
//
//             controller.handlePrintCommand(analysisType, analysisParams, outputVarsStr);
//             return true;
//         }
//         return false;
//     }
//
//     // New method to handle file-related commands
//     bool tryParseFileAndMenuCommands(const string& in) {
//         if (in == "show existing schematics") {
//             controller.handleShowExistingSchematicsCommand();
//             return true;
//         }
//         smatch matches;
//         regex new_file_pattern(R"(^NewFile\s+(\S+)$)");
//         if (regex_match(in, matches, new_file_pattern)) {
//             controller.handleNewFileCommand(matches[1].str());
//             return true;
//         }
//         // If in file menu, pass input to controller for handling
//         if(controller.isInFileMenu()){
//             controller.handleChooseSchematic(in, this);
//             return true;
//         }
//         return false;
//     }
//
//
// public:
//     explicit CommandParser(centralController& ctrl) : controller(ctrl) {}
//
//     void processInput(const string& in) {
//         if (trim(in).empty()) return;
//
//         string trimmed_in = trim(in);
//
//         // File menu commands have the highest priority
//         if (tryParseFileAndMenuCommands(trimmed_in)) return;
//
//         if (tryParseAnalysisDefinitionCommands(trimmed_in)) return;
//         if (tryParseNodeCommands(trimmed_in)) return;
//         if (tryParseListCommands(trimmed_in)) return;
//         if (tryParsePrintCommands(trimmed_in)) return;
//
//         // This is for direct user commands like 'add R1 ...'
//         if (tryParseElementCommands(trimmed_in)) return;
//         if (tryParseSourceCommands(trimmed_in)) return;
//
//         // This is for parsing lines from a file
//         if (tryParseNetlistLine(trimmed_in)) return;
//
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
//             if (!file.is_open()) {
//                 throw logic_error("Error: Could not open file " + filename);
//             }
//
//             clearCircuit(); // Clear current circuit before loading
//
//             cout << "Loading schematic '" << filename << "'..." << endl;
//             cout << filename << ".txt:" << endl;
//
//             string line;
//             while(getline(file, line)) {
//                 line = trim(line);
//                 if (line.empty() || line[0] == '*' || line[0] == '.') continue; // Ignore comments, .tran, etc.
//                 cout << line << endl; // Show file content as per PDF
//                 parser->processInput(line); // Process each line to build the circuit
//             }
//             file.close();
//             cout << "Schematic '" << filename << "' loaded successfully." << endl;
//
//         } else {
//             cout << "-Error: Inappropriate input" << endl;
//         }
//     } catch (const exception& e) {
//         cout << "-Error: Inappropriate input (" << e.what() << ")" << endl;
//     }
//
//     // Show menu again as per PDF behavior
//     handleShowExistingSchematicsCommand();
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
//     return 0;
// }

