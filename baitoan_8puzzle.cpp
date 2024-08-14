#include<bits/stdc++.h>

#define REP(i , l , r) for(int i = l ; i <= r ; i++)  // định nghĩa macro REP là vòng lặp for
#define REPD(i , l , r) for(int i = l ; i >= r ; i--)
#define REPS(i , l , r) for(int i = l ; i < r ; i++)

using namespace std;


enum facing { 
	LEFT , RIGHT , UP , DOWN , STAY
};
//định nghĩa tập gồm các hằng có kiểu số nguyên có tên facing

int puzzle[3][3] , posX , posY , checker;  // biến toàn cục
// khoi tao cac node nhu la cac trang thai cua puzzle

class node{
	public :
		int arr[3][3] , x , y;
		string way; 			//in ra đường đi đến đích
		facing canFace;
		
		node(int a[3][3] , string way , facing canFace , int x , int y){

			this->way = way;
			this->canFace = canFace;
			this->x = x;
			this->y = y;

			REP(i,0,2){ 		//for(int i=0; i<=2; i++)
				REP(j,0,2){
					arr[i][j] = a[i][j];
				}
			}
		}

		bool operator==(const node &other) const {
	        return memcmp(arr, other.arr, sizeof(arr)) == 0;
	    }

	    struct Hasher {
	        size_t operator()(const node& n) const {
	            size_t hash = 0;
	            REP(i, 0, 2) {
	                REP(j, 0, 2) {
	                    hash ^= hash * 31 + n.arr[i][j];
	                }
	            }
	            return hash;
	        }
	    };
		
		bool canMoveLeft(){
			return canFace != LEFT && y > 0;	// trả về true nếu canface khác left và nằm ở vị trí cột 2 hoặc 3
		}
		bool canMoveRight(){
			return canFace != RIGHT && y < 2;
		}
		bool canMoveUp(){
			return canFace != UP && x > 0;
		} 
		bool canMoveDown(){
			return canFace != DOWN && x < 2;
		}
		
		void moveLeft(){
			swap(arr[x][y] , arr[x][y-1]);		// phần tử 0 đổi vị trí cho phần tử phía bên trái
			y--;								// đặt lại tọa độ y cho phần tử 0 sau khi di chuyển 
			canFace = RIGHT;					// đặt trạng thái cho canface = right -> bước kế tiếp k đc di chuyển ngược lại sang phải
			way += "l";							// ghi đường đã đi
		}
		void moveRight(){
			swap(arr[x][y] , arr[x][y+1]);
			y++;
			canFace = LEFT;
			way += "r";
		}
		void moveUp(){
			swap(arr[x][y] , arr[x-1][y]);
			x--;
			canFace = DOWN;
			way += "u";
		}
		void moveDown(){
			swap(arr[x][y] , arr[x+1][y]);
			x++;
			canFace = UP;
			way += "d";
		}
		
		bool checkFinish(){			// kiểm tra các phần tử đã sắp xếp theo đúng thứ tự chưa
			if(checker == 1){	// 
				REP(i,0,2){			// kiểu 1
					if(arr[0][i] != i+1 || arr[2][i] != 7-i) return false;;
				}
				return arr[1][0] != 8 || arr[1][2] != 4 ? false : true;
			}else{
				REP(i,0,2){			// kiểu 2
					if(arr[0][i] != i+1 || arr[1][i] != i+4 ) return false;
				}
				return arr[2][0] != 7 || arr[2][1] != 8  || arr[2][2] != 0 ? false : true;
			}
			
		}
};

void moveLeft(){					// đổi chỗ và đặt lại tọa độ y cho phần tử 0
	swap(puzzle[posX][posY] , puzzle[posX][posY-1]);
	posY--;
}
void moveRight(){					// sử dụng các hàm này để in ra kết quả đường đi sau khi tìm kiếm xong!
	swap(puzzle[posX][posY] , puzzle[posX][posY+1]);
	posY++;
}
void moveUp(){
	swap(puzzle[posX][posY] , puzzle[posX-1][posY]);
	posX--;
}
void moveDown(){
	swap(puzzle[posX][posY] , puzzle[posX+1][posY]);
	posX++;
}

// Nhập và kiểm tra dữ liệu
void initPuzzle(){
	cout << "Nhap cac gia tri cho puzzle : ";
	cin >> puzzle[0][0] >> puzzle[0][1] >> puzzle[0][2];
	cin >> puzzle[1][0] >> puzzle[1][1] >> puzzle[1][2];
	cin >> puzzle[2][0] >> puzzle[2][1] >> puzzle[2][2];
	
	bool checked = true;	// kiểm tra mảng nhập có đúng định dạng k
	int sum = 0;			// tổng các phần tử -> phục vụ cho việc ktra
	REP(i,0,2){
		REP(j,0,2){
			sum += puzzle[i][j];
			if(puzzle[i][j] > 8) {
				checked = false;
				break;
			}
		}
	}
	set<int> s;		//cấu trúc dữ liệu 'set' lưu trữ các phần tử 
	REP(i,0,2){
		REP(j,0,2){
			if(s.find(puzzle[i][j])!=s.end()){ 	// kiểm tra xem phần tử đã có trong 'set' chưa
				checked = false;
				break;
			}
			s.insert(puzzle[i][j]);				// nếu chưa -> thêm vào 'set'
		}
	}	
	if(sum != 36 || checked == false){
		cout << "Nhap sai du lieu vui long nhap lai" << endl;
		return initPuzzle();
	}

// tìm kiếm vị trí của 0, gán giá trị cho posX, posY	
	REP(i,0,2){
		REP(j,0,2){
			if(puzzle[i][j] == 0){
				posX = i;
				posY = j;
				return;
			}
		}
	}
}

//In trạng thái 
void prin(){
	REP(i,0,2){
		REP(j,0,2){
			cout << puzzle[i][j] << " ";
		}
		cout << endl;
	}
}


// Kiểm tra xem là trạng thái đích chưa
bool checkFinish(){
	int counter1 = 0, counter2 = 0;

	// kiểu 1	
	REP(i,0,2){
		if(puzzle[0][i] == i+1) counter1++; 	// ktra hàng 1
	}	
	REP(i,0,2){
		if(puzzle[2][i] == 7-i) counter1++;		// ktra hàng 3
	}

	// kiểm tra hàng giữa
	if(puzzle[1][0] == 8){
		counter1++;
	}
	if(puzzle[1][2] == 4) {
		counter1++;
	}
	if(counter1 == 8) return true; // đúng hết thì trả về true

	// Kiểu 2		
	REP(i,0,2){
		if(puzzle[0][i] == i+1) counter2++;	// ktra hàng 1
		if(puzzle[1][i] == i+4) counter2++; // ktra hàng 2
		if(puzzle[2][i] == i+7) counter2++;	// ktra hàng 3
	}
		if(puzzle[2][2] == 0) counter2++;	// ktra
	if(counter2 == 8) return true;	// đúng hết thì trả về true

	return false;
}

//Xác định trạng thái đích là trường hợp 1 hay 2
int countStart(){  // 
	int sum = 0;

	REP(q,0,8){

		int row = q/3; 
		int col = q%3; 			
		int counter = puzzle[row][col];

		REP(i,0,2){
			REP(j,0,2){
				if( (row < i && puzzle[i][j] < counter && puzzle[i][j] != 0 )  ){	// tìm kiếm các cặp nghịch đảo ở bên dưới vị trí cần xét
					sum++;
				}else if(row == i && col < j && puzzle[i][j] < counter && puzzle[i][j] != 0){ 		// tìm kiếm các cặp nghịch đảo ở cùng hàng phía bên phải vị trí cần xét
				 	sum++;
				}
			}
		}
		
	}
	return sum; // trả về tổng số cặp nghịch đảo
}

int main(){
	//int step = 0 ;
	long long int numOfNode = 0;

	bool check = checkFinish();  // biến check -> trả về false khi trạng thái chưa phải là đích
	
	initPuzzle(); // nhập dl

	const clock_t begin_time = clock(); // ghi lại thời gian bắt đầu chạy ctrinh

	string way = "";	// ghi lại đường đi

	node start(puzzle , "" , STAY , posX , posY);	// tạo 1 đối tượng sart là trạng thái ban đầu

	vector<node> vt; // hàng đợi
	vt.push_back(start); //thêm trạng thái bắt đầu vào hàng đợi	

	unordered_set<node, node::Hasher> visited;	// lưu trữ các trạng thái đã được xét
    visited.insert(start);
	
	checker = countStart() %2; // biến checker sẽ xác định trạng thái đích

	cout << "\nTrang thai ban dau : " << endl;
	prin();
	cout << endl;

// tìm kiếm đường đi
	while(!check){
		vector<node> open;  // tạo 1 hàng đợi tạm thời lưu giữ các trạng thái mới được tạo ra từ trạng thái hiện tại
		int c = vt.size();
		
		REPS(i,0,c){
			numOfNode++;

			if(vt.at(i).checkFinish()){
				way = vt.at(i).way;
				check = true;
				break; //kết thúc vòng lặp
			}else{
				if(vt.at(i).canMoveLeft()){
					node nd = vt.at(i);
					nd.moveLeft();
					if(visited.find(nd)==visited.end())		// kiểm tra xem trạng thái mới này đã xuất hiện chưa
					{
						open.push_back(nd); // thêm trạng thái mới vào cuối open 
						visited.insert(nd);
					}				 	 
				}
				if(vt.at(i).canMoveRight()){
					node nd = vt.at(i);
					nd.moveRight();
					if(visited.find(nd)==visited.end())
					{
						open.push_back(nd); // thêm trạng thái mới vào cuối open 
						visited.insert(nd);
					}
				}
				if(vt.at(i).canMoveUp()){
					node nd = vt.at(i);
					nd.moveUp();
					if(visited.find(nd)==visited.end())
					{
						open.push_back(nd); // thêm trạng thái mới vào cuối open 
						visited.insert(nd);
					}
				}
				if(vt.at(i).canMoveDown()){
					node nd = vt.at(i);
					nd.moveDown();
					if(visited.find(nd)==visited.end())
					{
						open.push_back(nd); // thêm trạng thái mới vào cuối open 
						visited.insert(nd);
					}
				}
			}
			
		}
		vt.clear();		// xóa các trạng thái ở hàng đợi hiện tại
		int sop = open.size();
		REPS(i,0,sop){
			vt.push_back(open.at(i));	// thêm các trạng thái mới từ open vào hàng đợi
		}
	}
	
	int sway = way.length(); // tổng số bước đi (chiều dài của string way)

//in ra đường đi từ trạng thái bắt đầu đến đích
	REPS(i,0,sway){
		if(way[i] == 'l'){
			cout << "\n-> di chuyen sang trai" << endl;
			moveLeft();
			prin();	   
		}else if(way[i] == 'r'){
			cout << "\n-> di chuyen sang phai" << endl;
			moveRight();
			prin();			
		}else if(way[i] == 'u'){
			cout << "\n-> di chuyen len tren" << endl;
			moveUp();
			prin();			
		}else if(way[i] == 'd'){
			cout << "\n-> di chuyen xuong duoi" << endl ;
			moveDown();
			prin();			
		}
	}
	cout << "\nThuat toan BFS : " << endl;
	cout << "So buoc di = " << way.length() << endl;
	cout << "So phep toan da thuc hien = " << numOfNode << endl;
	cout << "Thoi gian tinh toan = " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << "s";
	return 0;
}