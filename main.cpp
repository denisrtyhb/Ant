#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <time.h>

#define right 0
#define up 1
#define left 2
#define down 3

using namespace std;

vector<sf::Color> colors = {
                            //sf::Color(255, 255, 204),
                            sf::Color(255, 0, 0),
                            //sf::Color(255, 255, 255),
                            //sf::Color(0, 0, 0),
                            sf::Color(0, 255, 0),
                            sf::Color(0, 0, 255),
                            sf::Color(127, 127, 0),
                            sf::Color(127, 0, 127),
                            sf::Color(0, 127, 127),
                            sf::Color(60, 60, 60),
                            sf::Color(200, 200, 200),
                            sf::Color(0, 255, 255),
                            sf::Color(255, 0, 255),
                            sf::Color(255, 255, 0)};
const int maxn = 5000;
const int global_size = 696;
int segment_size = 20;
int vision = 30;

int redraw_time = 10000;

const double pi = acos(-1);

bool _display_number_of_visits = false;

sf::Image _up;
sf::Texture _uptexture;
sf::Sprite _upsprite;

sf::Image _left;
sf::Texture _lefttexture;
sf::Sprite _leftsprite;

sf::Image _right;
sf::Texture _righttexture;
sf::Sprite _rightsprite;

sf::Image _down;
sf::Texture _downtexture;
sf::Sprite _downsprite;

sf::RenderWindow window(sf::VideoMode(global_size + 300, global_size + 20), "Ant", sf::Style::Close | sf::Style::Titlebar);
sf::Font font;

string toString(int x) {
    string res;
    res += '0' + (x % 10);
    x /= 10;
    while (x) {
        res += '0' + (x % 10);
        x /= 10;
    }
    reverse(res.begin(), res.end());
    return res;
}

void Sleep(long double x) {
    int f = clock() + x * CLOCKS_PER_SEC;
    while (f > clock()) {};
}

void Scale(int sz)
{
    vision = sz;
    segment_size = global_size / vision;
    if (segment_size < 3) segment_size = 3;
    vision = global_size / segment_size;
}

string s;
int nowx = maxn / 2, nowy = maxn / 2, nown;

vector<int> to;

sf::Text text;
int type = 1;

struct Board {
    int a[maxn][maxn];
    int number_of_visits[maxn][maxn];
    int ang_number_of_visits[maxn][maxn][4]; // 0 left down   1 left up   2 right up   3 right down
    bool changed[maxn][maxn];
    int posx, posy;
    //int type = 0;
    sf::RectangleShape tmp;

    Board() {
        for (int i = 0; i < maxn; ++i)
            for (int j = 0; j < maxn; ++j)
                a[i][j] = 0;
        for (int i = 0; i < maxn; ++i)
            for (int j = 0; j < maxn; ++j)
                changed[i][j] = false;
        posx = maxn / 2;
        posy = maxn / 2;
        tmp.setSize(sf::Vector2f(segment_size, segment_size));
    }

    void draw_segment(int x, int y) {

        if (segment_size >= 10)
        {
            tmp.setSize(sf::Vector2f(segment_size, segment_size));
            tmp.setPosition(10 + x * segment_size, 10 + y * segment_size);
            tmp.setFillColor(sf::Color(0, 0, 0));
            window.draw(tmp);
            tmp.setSize(sf::Vector2f(segment_size - 1, segment_size - 1));
            tmp.setPosition(11 + x * segment_size, 11 + y * segment_size);
            tmp.setFillColor(sf::Color(255, 255, 255));
            window.draw(tmp);
        }
        else
        {
            tmp.setSize(sf::Vector2f(segment_size, segment_size));
            tmp.setPosition(10 + x * segment_size, 10 + y * segment_size);
            tmp.setFillColor(sf::Color(255, 255, 255));
            window.draw(tmp);
        }
        if (type == 0)
        {
            if (segment_size >= 10)
            {
                tmp.setSize(sf::Vector2f(segment_size, segment_size));
                tmp.setPosition(10 + x * segment_size, 10 + y * segment_size);
                tmp.setFillColor(sf::Color(0, 0, 0));
                window.draw(tmp);
                tmp.setSize(sf::Vector2f(segment_size - 1, segment_size - 1));
                tmp.setPosition(11 + x * segment_size, 11 + y * segment_size);
                if (changed[x + posx - vision / 2][y + posy - vision / 2]) tmp.setFillColor(colors[a[x + posx - vision / 2][y + posy - vision / 2]]);
                else tmp.setFillColor(sf::Color(255, 255, 255));
                window.draw(tmp);
            }
            else
            {
                tmp.setSize(sf::Vector2f(segment_size, segment_size));
                tmp.setPosition(10 + x * segment_size, 10 + y * segment_size);
                if (changed[x + posx - vision / 2][y + posy - vision / 2]) tmp.setFillColor(colors[a[x + posx - vision / 2][y + posy - vision / 2]]);
                else tmp.setFillColor(sf::Color(255, 255, 255));
                window.draw(tmp);
            }
        }
        else if (type == 1)
        {
            bool wow = (to[a[x + posx - vision / 2][y + posy - vision / 2]] == 1 ? 0 : 1) ^ ((x + y) & 1) ^ 1;

            if (wow)
            {
                draw_arc(10 + x * segment_size + segment_size, 10 + y * segment_size,  10 + x * segment_size + segment_size / 2, 10 + y * segment_size);
                draw_arc(10 + x * segment_size + segment_size * 0, 10 + y * segment_size + segment_size,  10 + x * segment_size + segment_size / 2, 10 + y * segment_size + segment_size);
            }
            else
            {
                draw_arc(10 + x * segment_size, 10 + y * segment_size,  10 + x * segment_size, 10 + y * segment_size + segment_size / 2);
                draw_arc(10 + x * segment_size + segment_size, 10 + y * segment_size + segment_size,  10 + x * segment_size + segment_size, 10 + y * segment_size + segment_size / 2);
            }
        } else if (type == 2) {
        //if (_display_number_of_visits) {
            int px = 10 + x * segment_size;
            int py = 10 + y * segment_size;
            int ax = x + posx - vision / 2;
            int ay = y + posy - vision / 2;
            /*
            text.setString(to_string(number_of_visits[ax][ay]));
            text.setColor(sf::Color::Black);
            text.setPosition(px, py - 5);
            text.setCharacterSize(segment_size);
            window.draw(text);*/
            text.setString(to_string(ang_number_of_visits[ax][ay][0]));
            text.setColor(sf::Color::Black);
            text.setPosition(px, py + 3 * segment_size / 4);
            text.setCharacterSize(segment_size / 4);
            window.draw(text);

            text.setString(to_string(ang_number_of_visits[ax][ay][1]));
            text.setColor(sf::Color::Black);
            text.setPosition(px, py);
            text.setCharacterSize(segment_size / 4);
            window.draw(text);

            text.setString(to_string(ang_number_of_visits[ax][ay][2]));
            text.setColor(sf::Color::Black);
            text.setPosition(px + 2.5 * segment_size / 4, py);
            text.setCharacterSize(segment_size / 4);
            window.draw(text);

            text.setString(to_string(ang_number_of_visits[ax][ay][3]));
            text.setColor(sf::Color::Black);
            text.setPosition(px + 2.5 * segment_size / 4, py + 3 * segment_size / 4);
            text.setCharacterSize(segment_size / 4);
            window.draw(text);


        //}

        }
    }

    void draw_line(int x1, int y1, int x2, int y2, sf::Color col = sf::Color::Black) {
        sf::VertexArray line(sf::Lines, 2);
        line[0].position = sf::Vector2f(x1, y1);
        line[0].color = col;
        line[1].position = sf::Vector2f(x2, y2);
        line[1].color = col;
        window.draw(line);
    }

    void rotate(double &x, double &y, double alpha) {
        double tx = x * cos(alpha) - y * sin(alpha);
        double ty = x * sin(alpha) + y * cos(alpha);
        x = tx, y = ty;
    }

    void draw_arc(double x0, double y0, double x1, double y1) {
        x1 -= x0, y1 -= y0;
        //x2 -= x0, y2 -= y0;
        for (int i = 0; i < 5; ++i) {
            double tx1 = x1, ty1 = y1;
            rotate(x1, y1, -pi / 2 / 5);
            draw_line(x1 + x0, y1 + y0, tx1 + x0, ty1 + y0, sf::Color::Blue);
        }
    }

    void draw_ant() {
        //nowx = dx + posx - vision / 2;
        int dx = nowx - posx + vision / 2;
        int dy = nowy - posy + vision / 2;
        int px = 10 + dx * segment_size;
        int py = 10 + dy * segment_size;
        if (dx < 0 || vision <= dx || dy < 0 || vision <= dy) return;
        if (nown == up) {
            _upsprite.setPosition(px, py);
            _upsprite.setScale(segment_size * 1. / _upsprite.getLocalBounds().width,
                               segment_size * 1. / _upsprite.getLocalBounds().height);
            window.draw(_upsprite);
        }
        if (nown == right) {
            _rightsprite.setPosition(px, py);
            _rightsprite.setScale(segment_size * 1. / _rightsprite.getLocalBounds().width,
                               segment_size * 1. / _rightsprite.getLocalBounds().height);
            window.draw(_rightsprite);
        }
        if (nown == left) {
            _leftsprite.setPosition(px, py);
            _leftsprite.setScale(segment_size * 1. / _leftsprite.getLocalBounds().width,
                               segment_size * 1. / _leftsprite.getLocalBounds().height);
            window.draw(_leftsprite);
        }
        if (nown == down) {
            _downsprite.setPosition(px, py);
            _downsprite.setScale(segment_size * 1. / _downsprite.getLocalBounds().width,
                               segment_size * 1. / _downsprite.getLocalBounds().height);
            window.draw(_downsprite);
        }
    }

    int prevposx, prevposy, prevmousex, prevmousey;
    bool mouseon;
    bool onEvent(sf::Event &e) {
        bool res = false;
        if (e.type == sf::Event::MouseButtonPressed) {
            if (e.mouseButton.button == sf::Mouse::Left) {
                int mx = e.mouseButton.x, my = e.mouseButton.y;
                int l = 10; int r = l + segment_size * vision;
                if (l <= mx && mx <= r && l <= my && my <= r) {
                    mouseon = true;
                    prevposx = posx;
                    prevposy = posy;
                    prevmousex = mx;
                    prevmousey = my;
                }
            }
        }
        if (e.type == sf::Event::MouseButtonReleased) {
            if (e.mouseButton.button == sf::Mouse::Left) {
                mouseon = false;
            }
        }
        if (e.type == sf::Event::MouseMoved && mouseon) {
            posx = prevposx - (e.mouseMove.x - prevmousex) / segment_size;
            posy = prevposy - (e.mouseMove.y - prevmousey) / segment_size;
            res = true;
        }
        return res;
    }

    void draw() {
        //tmp.setSize(sf::Vector2f(segment_size * vision + 20, segment_size * vision + 20));
        tmp.setSize(sf::Vector2f(global_size + 320, global_size + 20));
        tmp.setFillColor(sf::Color(204, 255, 255));
        tmp.setPosition(sf::Vector2f(0, 0));
        window.draw(tmp);

        tmp.setSize(sf::Vector2f(segment_size * vision, segment_size * vision));
        tmp.setFillColor(sf::Color::Yellow);
        tmp.setPosition(sf::Vector2f(0, 0));
        //window.draw(tmp);

        for (int dx = 0; dx < vision; ++dx)
            for (int dy = 0; dy < vision; ++dy) {
                draw_segment(dx, dy);
            }
        draw_line(segment_size * vision + 11, 10, segment_size * vision + 11, segment_size * vision + 10);
        draw_line(10, segment_size * vision + 11, segment_size * vision + 10, segment_size * vision + 11);
        draw_line(segment_size * vision + 10, 10, segment_size * vision + 10, segment_size * vision + 10);
        draw_line(10, segment_size * vision + 10, segment_size * vision + 10, segment_size * vision + 10);
        draw_line(segment_size * vision + 10, 11, 10, 11);
        draw_line(11, segment_size * vision + 10, 11, 10);
        draw_line(segment_size * vision + 10, 10, 10, 10);
        draw_line(10, segment_size * vision + 10, 10, 10);
        draw_ant();
    }
};

Board board;

class TextField {
    public:
        TextField(unsigned int maxChars) : m_size(maxChars),
                                           m_rect(sf::Vector2f(15 * m_size, 20)), // 15 pixels per char, 20 pixels height, you can tweak
                                           m_hasfocus(false) {
            m_font.loadFromFile("TIMES.TTF");
            m_rect.setOutlineThickness(2);
            m_rect.setFillColor(sf::Color::White);
            m_rect.setOutlineColor(sf::Color(127, 127, 127));
            m_rect.setPosition(10, 10);

        }

        const std::string getText() const {
            return m_text;
        }

        void setPosition(float x, float y) {
            m_rect.setPosition(x, y);
        }
        void setText(string newtext) {
            m_text = newtext;
        }
        bool contains(sf::Vector2f point) const {
            return m_rect.getGlobalBounds().contains(point);
        }
        void setFocus(bool focus) {
            m_hasfocus = focus;
            if (focus) {
                m_rect.setOutlineColor(sf::Color::Blue);
            } else {
                m_rect.setOutlineColor(sf::Color(127, 127, 127)); // Gray color
            }
        }
        bool getFocus() {
            return m_hasfocus;
        }
        bool handleInput(sf::Event e) {
            if (!m_hasfocus || e.type != sf::Event::TextEntered)
                return false;
            if (e.text.unicode == 8) {   // Delete key
                if (!m_text.empty())
                    m_text.pop_back();
            }
            //else if (e.key.control && e.key.code == sf::Keyboard::V)
            //    m_text += sf::Clipboard::getString();
            //else if (e.key.control && e.key.code == sf::Keyboard::C)
            //    sf::Clipboard::setString(m_text);
            else if (toupper(e.text.unicode) == 'L' || toupper(e.text.unicode) == 'R') {//if (m_text.size() < m_size){
                m_text += toupper(e.text.unicode);
            }
            return true;
        }

        void draw() {
            window.draw(m_rect);
            sf::Text text;
            text.setFont(font);
            text.setString(m_text.substr(max(0, (int)m_text.size() - (int)m_size), m_size));
            text.setCharacterSize(20);
            text.setColor(sf::Color::Black);
            //cout << m_text << endl;
            text.setPosition(m_rect.getPosition().x, m_rect.getPosition().y - 5);
            window.draw(text);
        }

//        virtual void draw(sf::RenderTarget &ratget, sf::RenderStates states) const;

	private:
        unsigned int m_size;
        sf::Font m_font;
        std::string m_text;
        sf::RectangleShape m_rect;
        bool m_hasfocus;
};

int get_ind_nown(int was, int cur) { // return index to ang_number_of_visits
    if (was == right && cur == up)
        return 1;
    if (was == down && cur == left)
        return 1;
    if (was == right && cur == down)
        return 0;
    if (was == up && cur == left)
        return 0;
    if (was == down && cur == right)
        return 2;
    if (was == left && cur == up)
        return 2;
    if (was == up && cur == right)
        return 3;
    if (was == left && cur == down)
        return 3;
}

set<pair<int, int>> updated;

void step() {

    if (nowx < 0 || nowy < 0 || nowx >= maxn || nowy >= maxn) {
        return;
    }

    //cout << "was: " << nown << ' ' << board.a[nowx][nowy] << endl;
    int was = nown;
    nown = (nown + to[board.a[nowx][nowy]] + 4) % 4;
    board.a[nowx][nowy]++;
    board.number_of_visits[nowx][nowy]++;
    board.a[nowx][nowy] %= to.size();
    board.ang_number_of_visits[nowx][nowy][get_ind_nown(was, nown)]++;
    board.changed[nowx][nowy] = true;
    updated.insert({nowx, nowy});
    //cout << "now: " << nown << ' ' << board.a[nowx][nowy] << endl;
    if (nown == right)
        nowx += 1;
    else if (nown == left)
        nowx -= 1;
    else if (nown == up)
        nowy -= 1;
    else if (nown == down)
        nowy += 1;


    if (nowx < 0 || nowy < 0 || nowx >= maxn || nowy >= maxn) {
		cout << "The board is to small!!";
		//exit(0);
    }
}

void xsteps(int x) {
    for (int i = 0; i < x; ++i) {
        step();
    }
}

vector<string> split(string ss, char sep) {
    vector<string> res;
    res.push_back("");
    for (char cur : ss) {
        if (cur == sep) {
            res.push_back("");
        } else
            res.back() += cur;
    }
    return res;
}

void init_start() {
    ifstream in("start.csv");
    string cur;
    vector<vector<string>> res;
    while (getline(in, cur)) {
        res.push_back(split(cur, ';'));
        cout << cur << endl;
        cout << res.back().size() << endl;
    }
    cout << "!!!!" << endl;
    cout << res.size() << endl;
    int i1 = -1, j1;
    for (int i = 0; i < res.size(); ++i) {
        for (int j = 0; j < res[i].size(); ++j) {
                cout << res[i][j] << ' ';
            if (!res[i][j].empty() && res[i][j].back() == 'x') {
                i1 = i;
                j1 = j;
                res[i][j].pop_back();
            }
        }
        cout << endl;
    }
    cout << i1 << ' ' << j1 << endl;
    if (i1 == -1) return;
    //swap(i1, j1);
    for (int i = 0; i < res.size(); ++i) {
        for (int j = 0; j < res[i].size(); ++j) {
            if (!res[i][j].empty()) {
                board.a[nowx + j - j1][nowy + i - i1] = stoi(res[i][j]);
                board.changed[nowx + j - j1][nowy + i - i1] = true;
            }
        }
    }
    in.close();
}

void update() {

    for (auto ppos : updated) {
        int i = ppos.first;
        int j = ppos.second;
        board.a[i][j] = 0;
        board.changed[i][j] = false;
        board.number_of_visits[i][j] = 0;
        for (int k = 0; k < 4; ++k)
            board.ang_number_of_visits[i][j][k] = 0;
    }
    updated.clear();
/*    for (int i = 0; i < maxn; ++i)
        for (int j = 0; j < maxn; ++j)
            board.a[i][j] = 0;
    for (int i = 0; i < maxn; ++i)
        for (int j = 0; j < maxn; ++j)
            board.changed[i][j] = false;
    for (int i = 0; i < maxn; ++i)
        for (int j = 0; j < maxn; ++j)
            board.number_of_visits[i][j] = 0;
    for (int i = 0; i < maxn; ++i)
        for (int j = 0; j < maxn; ++j)
            for (int k = 0; k < 4; ++k)
                board.ang_number_of_visits[i][j][k] = 0;*/

    board.posx = board.posy = maxn / 2;
    nowx = nowy = maxn / 2;
    nown = 1;
    init_start();

    to.clear();
    for (char t : s) {
        if (t == 'R') to.push_back(-1);
        else to.push_back(1);
    }
}

bool ismouse_in(int tx, int ty, int szx, int szy) {

    int mouse_x = sf::Mouse::getPosition(window).x;
    int mouse_y = sf::Mouse::getPosition(window).y;
    cout << mouse_x << " " << mouse_y << endl;

    return tx <= mouse_x && mouse_x <= tx + szx && ty <= mouse_y && mouse_y <= ty + szy;
}

int parse_int(string &in) {
    int res = 0;
    for (int i = 0; i < in.size(); ++i) {
        if ('0' <= in[i] && in[i] <= '9') {
            res *= 10;
            res += in[i] - '0';
        }
    }
    for (int i = 0; i < in.size(); ++i)
        if (in[i] == '-') return -res;
    return res;
}

string parse_running_string(string &in) {
    string res;

    for (int i = 0; i < in.size(); ++i) {
        if (in[i] == 'L' || in[i] == 'R')
            res += in[i];
    }

    return res;
}

bool _x_steps = false;
int _x = 0;
bool _stop_flag = true;

void read_func() {
    while (true) {
        string input;
        getline(cin, input);

        if (input.find("stop") != string::npos)
            _stop_flag = true;
        if (input.find("start") != string::npos)
            _stop_flag = false;
        if (input.find("step") != string::npos) {
            _x_steps = true;
            _x = parse_int(input);
        }
        if (input.find("movex") != string::npos)
        {
            board.posx += parse_int(input);
        }
        if (input.find("movey") != string::npos)
        {
            board.posy += parse_int(input);
        }
        if (input.find("scale") != string::npos)
        {
            Scale(parse_int(input));
        }
        if (input.find("set_string") != string::npos) {
            s = parse_running_string(input);
            update();
        }
        if (input.find("numbers") != string::npos && input.find("on") != string::npos) {
            _display_number_of_visits = true;
        }
        if (input.find("numbers") != string::npos && input.find("off") != string::npos) {
            _display_number_of_visits = false;
        }
        if (input.find("type") != string::npos) {
            type = parse_int(input);
        }
    }
}



struct slider {
    sf::RectangleShape h, v;
    vector<string> vals;
    int prog = 0;
    bool mp = false;
    sf::Text text;
    slider() {}
    slider(int posx, int posy, vector<string> _vals) {
        vals = _vals;
        h.setPosition(posx, posy  + 29);
        h.setSize(sf::Vector2f(200, 3));
        v.setSize(sf::Vector2f(10, 57));
        v.setPosition(posx, posy + 1);
        v.setFillColor(sf::Color::Green);
        text.setPosition(posx + 50, posy + 65);
        text.setFont(font);
        text.setString(vals[0]);
        text.setColor(sf::Color::Black);
    }

    void change(int x) {
        int l = h.getPosition().x; int r = l + 200;
        if (x <= l)
            prog = 0;
        else if (x >= r)
            prog = (int)vals.size() - 1;
        else
            prog = round(1. * (x - l) * ((int)vals.size() - 1) / (r - l));
        //cout << l << " " << x << " " << r  << " " << prog << endl;

        v.setPosition(sf::Vector2f(round(l + 1. * prog * (r - l) / ((int)vals.size() - 1)), v.getPosition().y));
        text.setString(vals[prog]);
    }
    string get()
    {
        return vals[prog];
    }

    bool onEvent(sf::Event &e) {
        bool res = false;
        if (e.type == sf::Event::MouseButtonPressed) {
            if (e.mouseButton.button == sf::Mouse::Left) {
                int mx = e.mouseButton.x, my = e.mouseButton.y;
                int lx = h.getPosition().x; int rx = lx + 200;
                int ly = v.getPosition().y - 1; int ry = ly + 60;
                lx -= 20; rx += 20;
                if (lx <= mx && mx <= rx && ly <= my && my <= ry) {
                    mp = true;
                    change(e.mouseButton.x);
                    res = true;
                }
            }
        }
        if (e.type == sf::Event::MouseButtonReleased) {
            if (e.mouseButton.button == sf::Mouse::Left) {
                mp = false;
            }
        }
        if (e.type == sf::Event::MouseMoved) {
            if (mp) {
                change(e.mouseMove.x);
                res = true;
            }
        }
        return res;
    }



    void draw() {
        window.draw(h);
        window.draw(v);
        window.draw(text);
        //cout << h.getPosition().x << ' ' << h.getPosition().y << endl;
    }
};

slider s_board_size, s_step_count;
slider s_type(global_size + 30, 280, {"colors", "circles", "numbers"});

bool symb(char x)
{
    if ('a' <= x && x <= 'z') return true;
    if ('A' <= x && x <= 'Z') return true;
    if ('0' <= x && x <= '9') return true;
    return false;
}

vector<string> parse_config(string &s) {
    int i = 0;
    int n = s.size();
    for (i = 0; i < s.size() && s[i] != '='; ++i);
    ++i;
    vector<string> ans;
    string last = "";
    for (; i <= n; ++i) {
        if (i == n || s[i] == ',') {
            if (last.size()) {
                ans.push_back(last);
                last = "";
            }
        } else if (symb(s[i])) {
            last += s[i];
        }
    }
    return ans;
}

void init() {
    ifstream in("config.txt");
    string cur;
    while (getline(in, cur)) {
        auto gt = parse_config(cur);
        if (cur.find("size") != string::npos) {
            s_board_size = slider(global_size + 30, 0 + 30, gt);
        } else if (cur.find("steps") != string::npos) {
            s_step_count = slider(global_size + 30, 150, gt);
        } else if (cur.find("string") != string::npos) {
            s = gt[0];
            update();
        }
    }
    in.close();
}

int main() {
    init();
    Scale(30);
    sf::Thread thread(&read_func);
    thread.launch();

    _up.loadFromFile("up.png");
    _up.createMaskFromColor(sf::Color(255, 255, 255));
    _uptexture.loadFromImage(_up);
    _upsprite.setTexture(_uptexture);

    _right.loadFromFile("right.png");
    _right.createMaskFromColor(sf::Color(255, 255, 255));
    _righttexture.loadFromImage(_right);
    _rightsprite.setTexture(_righttexture);

    _left.loadFromFile("left.png");
    _left.createMaskFromColor(sf::Color(255, 255, 255));
    _lefttexture.loadFromImage(_left);
    _leftsprite.setTexture(_lefttexture);

    _down.loadFromFile("down.png");
    _down.createMaskFromColor(sf::Color(255, 255, 255));
    _downtexture.loadFromImage(_down);
    _downsprite.setTexture(_downtexture);

    if (!font.loadFromFile("TIMES.TTF")) {
        cout << "Font not found\n";
    }
    text.setFont(font);
    //s = "RL";
    //update();
    sf::Event event;

    sf::RectangleShape butmakemove;
    butmakemove.setPosition(global_size + 60, 210);
    butmakemove.setSize(sf::Vector2f(140, 50));

    //sf::RectangleShape butupdate;
    //butupdate.setPosition(global_size + 60, 400);
    //butupdate.setSize(sf::Vector2f(140, 50));

	TextField tf(15);
	tf.setPosition(global_size + 30, 600);
    sf::Text display_string;
    display_string.setPosition(global_size + 30, 500);
    display_string.setFont(font);
    display_string.setColor(sf::Color::Black);

    long long redraw_cnt = 0;
    bool changed = false;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                cout << "lol" << endl;
                window.close();
                exit(0);
                return 0;
            }
            if (event.type == event.MouseButtonPressed) {
                    //cout << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << '\n';
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                if (global_size + 60 <= x && x <= global_size + 140 + 60) {
                    if (210 <= y && y <= 210 + 50) {
                        int stepval = stoi(s_step_count.get());
                        xsteps(stepval);
                        changed = true;
                    }
                }
                if (global_size + 60 <= x && x <= global_size + 140 + 60) {
                    if (400 <= y && y <= 400 + 50) {
                        init();
                        changed = true;
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                auto pos = sf::Mouse::getPosition(window);
                bool was = tf.getFocus();
                tf.setFocus(false);
                if (tf.contains(sf::Vector2f(pos))){
                    tf.setFocus(true);
                }
                if (was != tf.getFocus()) {
                    changed = true;
                }
            } else {
                if (tf.handleInput(event)) {
                    s = tf.getText();
                    if (s.size())
                       update();
                    else
                        init();
                    changed = true;
                }
                //else if (tf.getText() != s) {
                //    tf.setText(s);
                //    changed = true;
                //}
            }
            changed |= s_board_size.onEvent(event);
            changed |= s_step_count.onEvent(event);
            changed |= s_type.onEvent(event);
            changed |= board.onEvent(event);
        }

        if (_x_steps) {
            _x_steps = false;
            xsteps(_x);
            changed = true;
        }

        if (!_stop_flag) {
            step();
            changed = true;
        }
        //Sleep(1);
        changed = true;
        if (redraw_cnt % redraw_time == 0 && changed) {
            changed = false;
            window.clear();
            vision = stoi(s_board_size.get()); Scale(vision);
            type = s_type.prog;
            board.draw();

            window.draw(butmakemove);
            //window.draw(butupdate);
            //sf::Text butupdate_text;
            //butupdate_text.setFont(font);
            //butupdate_text.setColor(sf::Color::Black);
            //butupdate_text.setPosition(global_size + 65, 405);
            //butupdate_text.setString("Update");
            //window.draw(butupdate_text);

            s_board_size.draw();
            s_step_count.draw();
            s_type.draw();

            display_string.setString(s);
            window.draw(display_string);
            tf.draw();
            window.display();
            //cout << tf.getText() << endl;
        }
        redraw_cnt += 1;
    }

    return 0;
}

