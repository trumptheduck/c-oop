#include <iostream>
#include <vector>
using namespace std;

//Class để lấy kích cỡ của sản phẩm, bao gồm tên<string> và trị số giá<float>
class Size {
    public:
    string name;
    float priceMultiplier;
    //Hàm khởi tạo, chạy khi tạo object mới từ class
    Size(string pName, float pMul) {
        this->name = pName;
        this->priceMultiplier = pMul;
    }
    //Nạp chồng thêm hàm copy vào hàm khởi tạo, để copy object
    Size(const Size& pSize) {
        this->name = pSize.name;
        this->priceMultiplier = pSize.priceMultiplier;
    }
};
//Class để lấy topping thêm cho đồ uống, bao gồm tên<string> và giá<int>
class Topping {
    public:
    string name;
    int price;
    //Hàm khởi tạo
    Topping(string pName, int pPrice) {
        this->name = pName;
        this->price = pPrice;
    }
};
//Class chung dành cho tất cả các mặt hàng
class Item {
    public: 
    //Tên mặt hàng
    string name;
    //Giá mặt hàng
    int price;
    //Hàm khởi tạo
    Item(string pName, int pPrice) {
        this->name = pName;
        this->price = pPrice;
    }
    //Hàm để in ra thông tin ban đầu của sản phẩm
    void introduction() {
        cout << "Item: " << this->name << endl;
    }
    //Hàm chung để chạy lệnh chọn sản phẩm, sẽ thay đổi ở những class con
    void select() {
        //Placeholder
    }
};
//Class chung cho những mặt hàng có kích cỡ
class SizableItem:public Item {
    public:
    //Vector là cấu trúc dữ liệu đặc biệt có tác dụng tương đương với mảng, nhưng có nhiều tiện ích hơn
    //Cú pháp: vector<Loại dữ liệu>()
    vector<Size> sizes[16]; 
    //Kích cỡ đã chọn
    Size selectedSize = *(new Size("Null",0));
    //Hàm khởi tạo, lấy vào tên, giá và các kích cỡ của mặt hàng, được lưu ở vector
    SizableItem(string pName, int pPrice, vector<Size> pSize[])
    : Item(pName,pPrice) {
        //Copy kích cỡ ở parameter vào cỡ ở trong object
        *sizes = *pSize;
    }
    //Hàm để chọn kích cỡ
    void selectSize() {
        int index = 0;
        //Lặp từ đầu vector(vector.begin()) đến cuối vector(vector.end()), lấy i làm pointer;
        for (auto i = sizes->begin(); i != sizes->end(); ++i) {
            index++;
            cout << "" << index << ": Size " << i->name << " | Price: "<<this->price*i->priceMultiplier<<"VND"<<endl;
        }
        index = 0;
        cout << "Select your preferred size: ";
        //Lấy input người dùng
        cin >> index;
        //Lấy kích cỡ người dùng chọn trong vector bằng vector.at(index);
        this->selectedSize = sizes->at(index-1);
    }
};
//Class đồ ăn, kế thừa mặt hàng có kích cỡ
class Food: public SizableItem{
    public:
    Food(string pName, int pPrice, vector<Size> pSize[])
    : SizableItem(pName,pPrice,pSize) { }
    //Hàm copy
    Food(const Food& pFood): SizableItem(pFood.name,pFood.price,new vector<Size>(*(pFood.sizes))) {
        this->selectedSize = *(new Size(pFood.selectedSize));
    }
    //Hàm lựa chọn
    void select() {
        //Đầu tiên in thông tin sản phẩm
        introduction();
        //Sau đó chọn kích cỡ bằng hàm kế thừa từ SizableItem
        selectSize();
    }
};
//Class đồ uông, kế thừa mặt hàng có kích cỡ
class Drink: public SizableItem{
    public:
    //Danh sách toppings
    vector<Topping> topping[16]; 
    //Các toppings đã chọn, tối đa 3
    vector<Topping> selectedToppings[3];

    Drink(string pName, int pPrice, vector<Size> pSize[], vector<Topping> pTopping[])
    : SizableItem(pName,pPrice,pSize) {
        *topping = *pTopping;
    }
    //Hàm copy
    Drink(const Drink& pDrink)
    : SizableItem(pDrink.name,pDrink.price,new vector<Size>(*(pDrink.sizes))) {
        this->selectedSize = *(new Size(pDrink.selectedSize));
        *topping = *(new vector<Topping>(*(pDrink.topping)));
        *selectedToppings = *(new vector<Topping>(*(pDrink.selectedToppings)));

    }
    //Hàm chọn toppings
    void selectToppings() {
        //Tương tự hàm chọn size, tuy vậy, đoạn sau có khác biệt
        int index = 0;
        for (auto i = topping->begin(); i != topping->end(); ++i) {
            index++;
            cout << "" << index << ": Size " << i->name << " | Price: "<<i->price<<"VND"<<endl;
        }
        //Tối đa 3 toppings
        int remaining = 3;
        cout << "Select your preferred toppings: " << endl;
        //Nếu remaining != 0 thì lặp
        while (remaining) {
            cout<< "Remaining: " << remaining << " | Choose< 0 to exit! >: ";
            cin >> index;
            //Nếu input là 0 lập tức dừng lại
            if (index == 0) break;
            //Đẩy topping vào cuối vector
            this->selectedToppings->push_back(topping->at(index-1));
            cout << "Selected: " << topping->at(index-1).name << " at " << topping->at(index-1).price << "VND" << endl;
            //Giảm remaining
            remaining--;
        }
    }
    //Hàm lựa chọn
    void select() {
        //In thông tin và chọn cỡ
        introduction();
        selectSize();
        //Chọn toppings
        selectToppings();
    }
};

class System {
    public:
    //Hàm tính tiền đồ ăn
    static int calculatePrice(Food pFood) {
        return pFood.price*pFood.selectedSize.priceMultiplier;
    }
    //Hàm tính tiền đồ uống (nạp chồng)
    static int calculatePrice(Drink pDrink) {
        int toppingsPrice = 0;
        for (auto i = pDrink.selectedToppings->begin(); i != pDrink.selectedToppings->end(); ++i) {
            toppingsPrice += i->price;
        }
        return pDrink.price*pDrink.selectedSize.priceMultiplier + toppingsPrice;
    }
};

int main() {
    //Danh sách đồ ăn
    vector<Food> *foodList = new vector<Food>({
        //Khai báo một món ăn
        *(new Food("Hamburger",20000,new vector<Size>({
            //Khai báo loại cỡ món ăn và trị số giá
            *(new Size("M",1.0f)),
            *(new Size("S",0.7f)),
            *(new Size("L",1.5f)),
        }))),
        *(new Food("Steak",100000,new vector<Size>({
            *(new Size("M",1.0f)),
            *(new Size("S",0.8f)),
            *(new Size("L",1.9f)),
        }))),
        *(new Food("Pasta",15000,new vector<Size>({
            *(new Size("M",1.0f)),
            *(new Size("S",0.8f)),
            *(new Size("L",1.5f)),
        }))),
    });
    vector<Drink> *drinkList = new vector<Drink>({
        *(new Drink("Milk Tea",25000,new vector<Size>({
            *(new Size("M",1.0f)),
            *(new Size("S",0.7f)),
            *(new Size("L",1.5f)),
            }), new vector<Topping>({
                //Khai báo loại topping
            *(new Topping("Boba",5000)),
            *(new Topping("Chocolate Chips",3000)),
            *(new Topping("Cream Cheese",7000)),
        })))
    });
    //Danh sách đồ ăn, uống đã chọn
    vector<Food> selectedFood;
    vector<Drink> selectedDrink;

    cout << "Welcome to our restaurant! Please take a look at the menu!"<<endl;
    while (true)
    {   //In ra tất cả món ăn
        cout <<"   - Food:"<<endl;
        int index = 0;
        for (auto i = foodList->begin(); i != foodList->end(); ++i) {
            index++;
            cout << "    + ";
            cout << "" << index << ": " << i->name << " | Price: "<<i->price<<"VND"<<endl;
        }
        //In ra tất cả đồ uống
        cout <<"   - Drink:"<<endl;
        //Tạo điểm ngắt giữa đồ ăn và đồ uống
        int breakPoint = index;
        for (auto i = drinkList->begin(); i != drinkList->end(); ++i) {
            index++;
            cout << "    + ";
            cout << "" << index << ": " << i->name << " | Price: "<<i->price<<"VND"<<endl;
        }
        cout << "Please choose an item< 0 to finish! >: ";
        cin >> index;
        if (index==0) break;
        //Nếu lựa chọn khách hàng thấp hơn hoặc bằng điểm ngắt, chọn đồ ăn
        if (index <= breakPoint) {
            Food *order = new Food(foodList->at(index-1));
            order->select();
            selectedFood.push_back(*(order));
            cout << "Selected " << order->name << "!" << endl;
        } else {
        //Nếu lựa chọn khách hàng cao hơn điểm ngắt, chọn đồ uống
        //Trừ đi điểm ngắt để lấy index đồ uống
        //Copy đồ uống bằng hàm copy
            Drink *order = new Drink(drinkList->at(index-1-breakPoint));
            //Chạy hàm select
            order->select();\
            //Đẩy đơn đặt đồ uống vào cuối vector danh sách đặt
            selectedDrink.push_back(*(order));
            cout << "Selected " << order->name << "!" << endl;
        }
        cout << "==============================" << endl;
    }
    //In hóa đơn
    int index = 0;
    int total = 0;
    cout << "=============BILL=============" << endl;
    cout << "You ordered: " << endl;
    for (auto i = selectedFood.begin(); i != selectedFood.end(); ++i) {
        index++;
        total += System::calculatePrice(*i);
        cout << "  + ";
        cout << "" << index << ": " << i->name << " | Price: "<<System::calculatePrice(*i)<<"VND"<<endl;
    }
    for (auto i = selectedDrink.begin(); i != selectedDrink.end(); ++i) {
        index++;
        total += System::calculatePrice(*i);
        cout << "  + ";
        cout << "" << index << ": " << i->name << " | Price: "<<System::calculatePrice(*i)<<"VND"<<endl;
    }
    cout << "Total: " << total << "VND" << endl;
    cout << "Thanks for using our services!" << endl;
    cout << "=============BILL=============" << endl;
    return 0;
}