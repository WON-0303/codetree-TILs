#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <queue>
using namespace std;

int N, M;

int dy[4] = { 1,0,0,-1 };
int dx[4] = { 0,1,-1,0 };

struct Mystr {
    int first;
    int second;
    int curr_y;
    int curr_x;
    int state = 0;
};

struct camp{
    int first;
    int second;
    int state = 0;
};

vector<Mystr> conv_store;
//vector<pair<int, int>> base_camp;
vector<camp> base_camp;

int base_camp_size = 0;

int map[17][17] = { 0, };

int calc(int y1, int x1, int y2, int x2) {
    return abs(y1 - y2) + abs(x1 - x2);
}

int visited[17][17] = { 0, };

struct parent {
    int first;
    int second;
};

//bfs 설계
parent route_bfs(int sy, int sx, int dest_y, int dest_x) {
    queue<pair<int, int>> q;
    memset(visited, 0, sizeof(visited));
    parent parent_map[17][17] = { {0, 0}, };
    int visited_parent_map[17][17] = { 0, };
    q.push({ sy,sx });
    visited[sy][sx] = 1;
    visited_parent_map[sy][sx] = 1;

    while (!q.empty()) {
        int y = q.front().first;
        int x = q.front().second;
        q.pop();
        for (int i = 3; i >= 0; --i) {
            int ny = y + dy[i];
            int nx = x + dx[i];
            if (ny < 0 || ny >= 17 || nx < 0 || nx >= 17) continue;
            if (map[ny][nx] != 2 && visited[ny][nx] == 0) {
                q.push({ ny,nx });
                visited[ny][nx] = 1;
                if (visited_parent_map[ny][nx] == 0)
                {
                    parent_map[ny][nx] = { y,x };
                    visited_parent_map[ny][nx] = 1;
                }

                if (ny == dest_y && nx == dest_x) {
                    //cout<<"clear\n";
                    //return {1,1};
                    while (1) {
                        y = ny;
                        x = nx;
                        ny = parent_map[y][x].first;
                        nx = parent_map[y][x].second;

                        if (ny == sy && nx == sx) {
                            parent tmp;
                            tmp.first = y;
                            tmp.second = x;
                            return tmp;
                        }
                    }
                }
            }


        }
    }


}

int dist_bfs(int sy, int sx, int dest_y, int dest_x) {
    queue<pair<int, int>> q;
    memset(visited, 0, sizeof(visited));
    int dist_map[17][17] = { 0, };
    q.push({ sy,sx });
    visited[sy][sx] = 1;
    dist_map[sy][sx] = 0;

    while (!q.empty()) {
        int y = q.front().first;
        int x = q.front().second;
        q.pop();
        for (int i = 3; i >= 0; --i) {
            int ny = y + dy[i];
            int nx = x + dx[i];
            if (map[ny][nx] != 2 && visited[ny][nx] == 0) {
                q.push({ ny,nx });
                visited[ny][nx] = 1;
                dist_map[ny][nx] = dist_map[y][x] + 1;
            }
            if (ny == dest_y && nx == dest_x) {
                //cout<<"dist_bfs: " <<dist_map[dest_y][dest_x]<<"\n";
                return dist_map[dest_y][dest_x];
            }
        }
    }

    return 999999;


}

//1
void move(int num) { //num번째 편의점 가고자하는사람
    //현재좌표
    int y = conv_store[num].curr_y;
    int x = conv_store[num].curr_x;
    //최단거리 계산
    // int curr_dist = calc(y,x,conv_store[num].first,conv_store[num].second);
    // //하우좌상 최단거리 탐색
    // for(int i = 0; i<4; ++i){
    //     int ny = conv_store[num].curr_y + dy[i];
    //     int nx = conv_store[num].curr_x + dx[i];
    //     int n_curr_dist = calc(ny,nx,conv_store[num].first,conv_store[num].second);
    //     if(n_curr_dist<=curr_dist && map[ny][nx] != 2){
    //         curr_dist = n_curr_dist;
    //         y = ny;
    //         x = nx;
    //     }
    // }

    parent location = route_bfs(y, x, conv_store[num].first, conv_store[num].second);
    //parent location = {1,1};
    //최단거리로 위치 이동
    conv_store[num].curr_y = location.first;
    conv_store[num].curr_x = location.second;
}

//2
void check_conv_store(int num) {
    //편의점에 도착
    if (conv_store[num].curr_y == conv_store[num].first && conv_store[num].curr_x == conv_store[num].second) {
        //해당편의점 map = 2로 변경
        //중요! 격자에 있는 사람들이 모두 이동하고 나서야 편의점 도착 체크 필요
        map[conv_store[num].first][conv_store[num].second] = 2;
        //편의점에 도착한 사람 vector 지워버리기
        //conv_store.erase(conv_store.begin()+num);
        conv_store[num].state = 1;

        //cout<<"check: "<<num<<"\n";
    }
}

//3
void check_time(int num) {
    //자기 편의점과 제일 가까운 베이스캠프 찾기, 해당 좌표 curr값에 넣기
    int y = conv_store[num].first;
    int x = conv_store[num].second;
    int min_dist = 999999;

    for (int i = 0; i < base_camp_size; ++i) {
        if(base_camp[i].state == 0)
        {
        int base_camp_y = base_camp[i].first;
        int base_camp_x = base_camp[i].second;
        //베이스캠프간 거리 탐색
        //int dist = calc(y,x,base_camp_y,base_camp_x);
        int dist = dist_bfs(y, x, base_camp_y, base_camp_x);
        //cout<<y<<" "<<x<<" "<<base_camp_y<<" "<<base_camp_x<<"\n";
        if (min_dist > dist) {
            min_dist = dist;
            conv_store[num].curr_y = base_camp_y;
            conv_store[num].curr_x = base_camp_x;
        }


        //가장 가까운 베이스캠프 여러개
        else if (min_dist == dist) {
            if (base_camp_y < conv_store[num].curr_y) {
                conv_store[num].curr_y = base_camp_y;
                conv_store[num].curr_x = base_camp_x;
            }
            else if (base_camp_y == conv_store[num].curr_y) {
                if (base_camp_x < conv_store[num].curr_x) {
                    conv_store[num].curr_y = base_camp_y;
                    conv_store[num].curr_x = base_camp_x;
                }
            }
        }

    }
    }
    // conv_store[num].curr_y = y;
    // conv_store[num].curr_x = x;

    //베이스캠프에 진입, 해당 베이스캠프 erase하기, 해당 베이스캠프 map = 2로 변경
    map[conv_store[num].curr_y][conv_store[num].curr_x] = 2;
    for (int i = 0; i < base_camp_size; ++i) {
        if(base_camp[i].state == 0){
            if(base_camp[i].first == conv_store[num].curr_y && base_camp[i].second == conv_store[num].curr_x){
                base_camp[i].state = 1;
                break;
            }
        }
    }
    


}

int main() {
    cin >> N >> M;
    for (int i = 0; i <= N + 1; ++i) {
        for (int j = 0; j <= N + 1; ++j) {
            map[i][j] = 2; //2가 벽
        }
    }

    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            int input_tmp;
            cin >> input_tmp;
            map[i][j] = input_tmp;
            if (input_tmp == 1) {
                base_camp.push_back({ i,j,0 });
                base_camp_size++;
            }
        }
    }

    for (int i = 0; i < M; ++i) {
        int y_tmp, x_tmp;
        cin >> y_tmp >> x_tmp;
        conv_store.push_back({ y_tmp,x_tmp,16,16,0 });
    }

    int clear_time = 1;
    int curr_time = 0;


    int cnt = 0;
        //for (int k = 0; k < 3; ++k) {
        while(1){
            //3, 베이스캠프에 사람 넣기
        if (curr_time < M) {
            if (conv_store[curr_time].state == 0) {
                check_time(curr_time);
                //cout<<"y,x: "<<conv_store[curr_time].curr_y<<" "<<conv_store[curr_time].curr_x<<"\n";
            }
        }



        //1, 사람들 이동
        for (int i = 0; i <= min(M - 1, curr_time); ++i) {
            if (conv_store[i].state == 0) {
                //cout<<"move_run\n";
                move(i);

            }
        }

        //2, 편의점 도착 체크
        for (int i = 0; i <= min(M - 1, curr_time); ++i) {
            if (conv_store[i].state == 0) {
                check_conv_store(i);
            }
        }



        // int temp_map[17][17] = {0,};
        // for(int i=0; i<=min(curr_time,M-1); ++i){
        //     temp_map[conv_store[i].curr_y][conv_store[i].curr_x] = 1;
        // }
        // for(int i=1; i<=N; ++i){
        //     for(int j=1; j<=N; ++j){
        //         cout<<temp_map[i][j]<<" ";
        //     }
        //     cout<<"\n";
        // }
        // cout<<"\n";

        // memset(temp_map,0,sizeof(temp_map));
        // for(int i=0; i<=min(curr_time,M-1); ++i){
        //     temp_map[conv_store[i].first][conv_store[i].second] = 2;
        // }
        // for(int i=1; i<=N; ++i){
        //     for(int j=1; j<=N; ++j){
        //         cout<<temp_map[i][j]<<" ";
        //     }
        //     cout<<"\n";
        // }
        // cout<<"\n";

        curr_time++;
        clear_time++;

        //탈출조건
        int cnt = 0;
        for (int i = 0; i < M; ++i) {

            if (conv_store[i].state == 1) {
                cnt++;
            }
            if (cnt == M) {
                cout << clear_time;
                return 0;
            }
        }

    }
    // cout<<clear_time;

    return 0;
}