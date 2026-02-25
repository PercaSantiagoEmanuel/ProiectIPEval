#include<iostream>
#include<stack>
#include<vector>
#include<algorithm>
#include<cmath>
#include<limits.h>
#include<string>
#include<map>
#include<graphics.h>
#include<cstdio>

using namespace std;

#define infinit 1e18
#define LIMITA_JOS_ARBORE 620
#define pi 3.14159265
#define e 2.71828182

string mesajEroareMatematica = "";
bool eroare = false;

struct NodArbore {
    string valoare;
    NodArbore* stg, * dr;
    float x, y;
    NodArbore(string val) : valoare(val), stg(nullptr), dr(nullptr), x(0), y(0) {}
};

bool DiferitInfinit(float x) {
    return isfinite(x) && fabs(x) < infinit;
}

float Plus(float x, float y) {
    if (eroare) return 0;
    return x + y;
}

float Minus(float x, float y) {
    if (eroare) return 0;
    return x - y;
}

float Inmultit(float x, float y) {
    if (eroare) return 0;
    return x * y;
}

float Impartit(float x, float y) {
    if (eroare) return 0;
    if (isinf(y)) {
        mesajEroareMatematica = "Eroare: Impartire la infinit!";
        eroare = true;
        return 0;
    }
    if (abs(y) < 0.0001) {
        mesajEroareMatematica = "Eroare: Impartire la zero!";
        eroare = true;
        return infinit;
    }
    return x / y;
}

float radical(float x) {
    if (eroare) return infinit;
    if (x < 0) {
        mesajEroareMatematica = "Radical din negativ (Calculat cu modul)";
        return sqrt(abs(x));
    }
    return sqrt(x);
}

float logaritm(float x) {
    if (eroare) return infinit;
    if (x <= 0.0001) {
        mesajEroareMatematica = "Logaritm din <= 0 (Calculat cu modul)";
        return log(abs(x));
    }
    return log(x);
}

float sinus(float x) {
    if (eroare) return 0;
    return sin(x);
}

float cosinus(float x) {
    if (eroare) return 0;
    return cos(x);
}

float modul(float x) {
    if (eroare) return 0;
    return abs(x);
}

float exponential(float x) {
    if (eroare) return 0;
    return exp(x);
}

float putere(float x, float y) {
    if (eroare) return 0;
    return pow(x, y);
}

float MaiMic(float x, float y) {
    return (x < y) ? 1.0f : 0.0f;
}

float MaiMare(float x, float y) {
    return (x > y) ? 1.0f : 0.0f;
}

float Egal(float x, float y) {
    return (abs(x - y) < 0.0001) ? 1.0f : 0.0f;
}

float Diferit(float x, float y) {
    return (abs(x - y) >= 0.0001) ? 1.0f : 0.0f;
}

bool Functie(string functie) {
    const vector<string> este = { "sin", "cos", "tg", "ctg", "abs", "exp", "log", "sqr" };
    return find(este.begin(), este.end(), functie) != este.end();
}

int PrioritateOperator(const string& op) {
    if (Functie(op)) return 6;
    if (op == "^") return 4;
    if (op == "*" || op == "/") return 3;
    if (op == "+" || op == "-") return 2;
    if (op == "<" || op == ">" || op == "=" || op == "#") return 1;
    return -1;
}

bool esteOperator(string op) {
    return (op == "^" || op == "*" || op == "/" || op == "+" || op == "-" || op == "<" || op == ">" || op == "=" || op == "#");
}

void Despartire(string expresie, vector<string>& ExpresieFinal) {
    size_t i = 0;
    while (i < expresie.length()) {
        if (isspace(expresie[i])) {
            i++;
            continue;
        }
        if (expresie[i] == '-' && (ExpresieFinal.empty() || ExpresieFinal.back() == "(")) {
            ExpresieFinal.push_back("(");
            ExpresieFinal.push_back("0");
            ExpresieFinal.push_back("-");
            i++;
            if (isdigit(expresie[i])) {
                string nr;
                while (i < expresie.length() && (isdigit(expresie[i]) || expresie[i] == '.')) nr.push_back(expresie[i++]);
                ExpresieFinal.push_back(nr);
            }
            else if (isalpha(expresie[i])) {
                string word;
                while (i < expresie.length() && isalpha(expresie[i])) word.push_back(expresie[i++]);
                ExpresieFinal.push_back(word);
            }
            ExpresieFinal.push_back(")");
        }
        else if (isdigit(expresie[i])) {
            string nr;
            while (i < expresie.length() && (isdigit(expresie[i]) || expresie[i] == '.')) nr.push_back(expresie[i++]);
            ExpresieFinal.push_back(nr);
        }
        else if (isalpha(expresie[i])) {
            string word;
            while (i < expresie.length() && isalpha(expresie[i])) word.push_back(expresie[i++]);
            ExpresieFinal.push_back(word);
        }
        else {
            ExpresieFinal.push_back(string(1, expresie[i++]));
        }
    }
}

void Algoritm_Shunting_Yard(vector<string> ExpresieFinal, vector<string>& ExpresieShunting) {
    stack<string> Op;
    for (const string& token : ExpresieFinal) {
        if (isdigit(token[0]) || (isalpha(token[0]) && !Functie(token))) ExpresieShunting.push_back(token);
        else if (Functie(token) || token == "(") Op.push(token);
        else if (token == ")") {
            while (!Op.empty() && Op.top() != "(") {
                ExpresieShunting.push_back(Op.top());
                Op.pop();
            }
            if (!Op.empty()) Op.pop();
            if (!Op.empty() && Functie(Op.top())) {
                ExpresieShunting.push_back(Op.top());
                Op.pop();
            }
        }
        else if (esteOperator(token)) {
            while (!Op.empty() && Op.top() != "(" && PrioritateOperator(Op.top()) >= PrioritateOperator(token)) {
                ExpresieShunting.push_back(Op.top());
                Op.pop();
            }
            Op.push(token);
        }
    }
    while (!Op.empty()) {
        ExpresieShunting.push_back(Op.top());
        Op.pop();
    }
}

NodArbore* ConstruireArbore(const vector<string>& ExpresieShunting) {
    if (ExpresieShunting.empty()) return nullptr;
    stack<NodArbore*> stiva;
    for (const string& token : ExpresieShunting) {
        NodArbore* nou = new NodArbore(token);
        if (isdigit(token[0]) || (isalpha(token[0]) && !Functie(token))) stiva.push(nou);
        else if (Functie(token)) {
            if (!stiva.empty()) {
                nou->stg = stiva.top();
                stiva.pop();
            }
            stiva.push(nou);
        }
        else {
            if (stiva.size() >= 2) {
                nou->dr = stiva.top();
                stiva.pop();
                nou->stg = stiva.top();
                stiva.pop();
            }
            stiva.push(nou);
        }
    }
    return stiva.empty() ? nullptr : stiva.top();
}

double CalculeazaExpresia(vector<string> ExpresieShunting, const map<string, double>& variabile) {
    stack<double> stiva;
    eroare = false;
    mesajEroareMatematica = "";

    for (const string& token : ExpresieShunting) {
        if (isdigit(token[0])) stiva.push(stod(token));
        else if (isalpha(token[0]) && !Functie(token)) stiva.push(variabile.at(token));
        else if (Functie(token)) {
            if (stiva.empty()) continue;
            double val = stiva.top();
            stiva.pop();
            if (token == "sin") stiva.push(sinus(val));
            else if (token == "cos") stiva.push(cosinus(val));
            else if (token == "abs") stiva.push(modul(val));
            else if (token == "exp") stiva.push(exponential(val));
            else if (token == "log") stiva.push(logaritm(val));
            else if (token == "sqr") stiva.push(radical(val));
        }
        else {
            if (stiva.size() < 2) continue;
            double dr = stiva.top();
            stiva.pop();
            double st = stiva.top();
            stiva.pop();
            if (token == "+") stiva.push(Plus(st, dr));
            else if (token == "-") stiva.push(Minus(st, dr));
            else if (token == "*") stiva.push(Inmultit(st, dr));
            else if (token == "/") stiva.push(Impartit(st, dr));
            else if (token == "^") stiva.push(putere(st, dr));
            else if (token == "<") stiva.push(MaiMic(st, dr));
            else if (token == ">") stiva.push(MaiMare(st, dr));
            else if (token == "=") stiva.push(Egal(st, dr));
            else if (token == "#") stiva.push(Diferit(st, dr));
        }
    }
    return stiva.empty() ? 0 : stiva.top();
}

bool VerificaExpresieAvansat(const string& expr, string& mesaj) {
    if (expr.empty()) {
        mesaj = "Expresia este goala.";
        return false;
    }
    int par = 0;
    bool asteaptaOperand = true;
    bool valid = true;
    string listaErori = "";
    char lastOp = 0;
    vector<string> fStack;
    string pendingFunc = "";
    bool isNeg = false;

    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];
        if (isspace(c)) continue;

        if (isdigit(c)) {
            if (!asteaptaOperand) {
                listaErori += "Lipseste op. intre valori | ";
                valid = false;
            }
            string nStr;
            int puncte = 0;
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.')) {
                if (expr[i] == '.') puncte++;
                nStr += expr[i];
                i++;
            }
            i--;
            if (puncte > 1) {
                listaErori += "Nr cu prea multe puncte | ";
                valid = false;
            }

            try {
                double val = stod(nStr);
                if (isNeg) val = -val;

                if (lastOp == '/' && val == 0.0) {
                    listaErori += "Impartire la 0 | ";
                    valid = false;
                }
                if (isinf(val)) {
                    listaErori += "Valoare infinita | ";
                    valid = false;
                }

                if (!fStack.empty()) {
                    string f = fStack.back();
                    if ((f == "log" || f == "ln" || f == "lg") && val <= 0) {
                        listaErori += "Log din <= 0 | ";
                        valid = false;
                    }
                    if ((f == "sqrt" || f == "sqr") && val < 0) {
                        listaErori += "Sqrt din negativ | ";
                        valid = false;
                    }
                }
            }
            catch (...) {
                listaErori += "Format numar invalid | ";
                valid = false;
            }

            asteaptaOperand = false;
            lastOp = 0;
            isNeg = false;
        }
        else if (isalpha(c)) {
            if (!asteaptaOperand) {
                listaErori += "Lipseste op. inainte de var | ";
                valid = false;
            }
            string nume;
            while (i < expr.length() && isalpha(expr[i])) {
                nume += expr[i];
                i++;
            }
            i--;

            if ((nume == "inf" || nume == "infinit" || nume == "infinity") && lastOp == '/') {
                listaErori += "Impartire la infinit nepermisa | ";
                valid = false;
            }

            if (Functie(nume)) {
                size_t j = i + 1;
                while (j < expr.length() && isspace(expr[j])) j++;
                if (j >= expr.length() || expr[j] != '(') {
                    listaErori += "Functie fara '(' | ";
                    valid = false;
                }
                pendingFunc = nume;
            }
            else {
                asteaptaOperand = false;
            }
            lastOp = 0;
            isNeg = false;
        }
        else if (string("+-*/^<>=#").find(c) != string::npos) {
            if (c == '-' && (i + 1 < expr.length() && expr[i + 1] == '-')) {
                listaErori += "-- nepermis | ";
                valid = false;
            }

            if (asteaptaOperand && c == '-') {
                isNeg = true;
            }
            else {
                if (asteaptaOperand) {
                    listaErori += "Operator la inceput/dublu | ";
                    valid = false;
                }
                asteaptaOperand = true;
                lastOp = c;
                isNeg = false;
            }
        }
        else if (c == '(') {
            if (!asteaptaOperand) {
                listaErori += "Lipseste op. | ";
                valid = false;
            }
            par++;
            asteaptaOperand = true;
            fStack.push_back(pendingFunc);
            pendingFunc = "";
            lastOp = 0;
        }
        else if (c == ')') {
            if (asteaptaOperand) {
                listaErori += "Op. inainte de ) | ";
                valid = false;
            }
            par--;
            if (par < 0) {
                listaErori += ") in plus | ";
                valid = false;
                par = 0;
            }
            else {
                if (!fStack.empty()) fStack.pop_back();
            }

            asteaptaOperand = false;
            lastOp = 0;
            isNeg = false;
        }
        else {
            listaErori += "Caracter invalid | ";
            valid = false;
        }
    }
    if (par != 0) {
        listaErori += "Paranteze neinchise | ";
        valid = false;
    }
    if (asteaptaOperand) {
        listaErori += "Se termina cu operator | ";
        valid = false;
    }
    if (!valid) {
        mesaj = listaErori;
        return false;
    }
    return true;
}

void SimplificaArboreVizual(NodArbore* nod) {
    if (!nod) return;
    SimplificaArboreVizual(nod->stg);
    SimplificaArboreVizual(nod->dr);
    if (nod->valoare == "-" && nod->stg && nod->stg->valoare == "0") {
        NodArbore* nodValoare = nod->dr;
        if (nodValoare && !nodValoare->stg && !nodValoare->dr) {
            nod->valoare = "-" + nodValoare->valoare;
            delete nod->stg;
            delete nod->dr;
            nod->stg = nullptr;
            nod->dr = nullptr;
        }
    }
}

string getClipboardText() {
    if (!OpenClipboard(nullptr)) return "";
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (!hData) {
        CloseClipboard();
        return "";
    }
    char* pszText = static_cast<char*>(GlobalLock(hData));
    string text = pszText ? pszText : "";
    GlobalUnlock(hData);
    CloseClipboard();
    return text;
}

string citireGrafica(int x, int y, int width_limit) {
    string text = "";
    char tasta;
    char afisaj[500];

    int paginaVeche = getactivepage();
    setactivepage(getvisualpage());

    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
    setbkcolor(LIGHTGRAY);
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(x, y, x + width_limit, y + 40);
    outtextxy(x, y, (char*)text.c_str());

    while (true) {
        if (kbhit()) {
            tasta = getch();
            if (tasta == 13) break;
            if (tasta == 22) text += getClipboardText();
            else if (tasta == 8 && !text.empty()) text.pop_back();
            else if (tasta >= 32 && tasta <= 126) text += tasta;

            setfillstyle(SOLID_FILL, LIGHTGRAY);
            bar(x, y, x + width_limit, y + 40);
            setbkcolor(LIGHTGRAY);
            strcpy(afisaj, text.c_str());
            outtextxy(x, y, afisaj);
        }
        delay(10);
    }

    setactivepage(paginaVeche);
    return text;
}

void stergereMemorie(NodArbore* nod) {
    if (!nod) return;
    stergereMemorie(nod->stg);
    stergereMemorie(nod->dr);
    delete nod;
}

int getInaltime(NodArbore* r) {
    if (!r) return 0;
    return 1 + max(getInaltime(r->stg), getInaltime(r->dr));
}

void SeteazaPozitiiInitiale(NodArbore* r, int x, int y, int dist, int pasY) {
    if (!r) return;
    r->x = x;
    r->y = y;
    int distMinima = 30;
    int distUrmatoare = (dist / 2 < distMinima) ? distMinima : dist / 2;
    if (y > LIMITA_JOS_ARBORE) r->y = LIMITA_JOS_ARBORE;
    if (r->valoare == "-" && r->stg == NULL && r->dr != NULL) {
        SeteazaPozitiiInitiale(r->dr, x, y + pasY, dist, pasY);
    }
    else if (Functie(r->valoare)) {
        if (r->stg) SeteazaPozitiiInitiale(r->stg, x, y + pasY, dist, pasY);
    }
    else {
        if (r->stg) SeteazaPozitiiInitiale(r->stg, x - dist, y + pasY, distUrmatoare, pasY);
        if (r->dr) SeteazaPozitiiInitiale(r->dr, x + dist, y + pasY, distUrmatoare, pasY);
    }
}

void DeseneazaButon(int x, int y, int w, int h, char* text, int culoareFundal, int culoareText) {
    setfillstyle(SOLID_FILL, culoareFundal);
    bar(x, y, x + w, y + h);
    setcolor(culoareText);
    setbkcolor(culoareFundal);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    settextjustify(LEFT_TEXT, TOP_TEXT);
    outtextxy(x + (w - textwidth(text)) / 2, y + (h - textheight(text)) / 2, text);
}

void DeseneazaDinamic(NodArbore* r) {
    if (!r) return;
    setcolor(BLACK);
    setlinestyle(SOLID_LINE, 0, 1);
    if (r->stg) {
        line(r->x, r->y, r->stg->x, r->stg->y);
        DeseneazaDinamic(r->stg);
    }
    if (r->dr) {
        line(r->x, r->y, r->dr->x, r->dr->y);
        DeseneazaDinamic(r->dr);
    }

    char buffer[100];
    strcpy(buffer, r->valoare.c_str());
    int raza = (textwidth(buffer) / 2) + 7;
    if (raza < 18) raza = 18;
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(r->x, r->y, raza, raza);
    circle(r->x, r->y, raza);
    setbkcolor(WHITE);
    outtextxy(r->x - textwidth(buffer) / 2, r->y - textheight(buffer) / 2 + 2, buffer);
}

NodArbore* GasesteNod(NodArbore* nod, int mx, int my) {
    if (!nod) return nullptr;
    if (sqrt(pow(nod->x - mx, 2) + pow(nod->y - my, 2)) < 30) return nod;
    NodArbore* gasit = GasesteNod(nod->stg, mx, my);
    return gasit ? gasit : GasesteNod(nod->dr, mx, my);
}

void ColecteazaNoduri(NodArbore* nod, vector<NodArbore*>& lista) {
    if (!nod) return;
    lista.push_back(nod);
    ColecteazaNoduri(nod->stg, lista);
    ColecteazaNoduri(nod->dr, lista);
}

void RezolvaSuprapuneri(NodArbore* rad) {
    if (!rad) return;
    vector<NodArbore*> noduri;
    ColecteazaNoduri(rad, noduri);
    float razaMinima = 30.0f;
    float distantaMinima = razaMinima * 2.0f;

    for (size_t i = 0; i < noduri.size(); i++) {
        for (size_t j = i + 1; j < noduri.size(); j++) {
            NodArbore* n1 = noduri[i];
            NodArbore* n2 = noduri[j];
            float dx = n1->x - n2->x;
            float dy = n1->y - n2->y;
            float dist = sqrt(dx * dx + dy * dy);

            if (dist < distantaMinima) {
                if (dist == 0) {
                    dist = 0.1f;
                    dx = 1.0f;
                }
                float suprapunere = distantaMinima - dist;
                float nx = dx / dist;
                float ny = dy / dist;
                float forta = 0.2f;
                n1->x += nx * suprapunere * forta;
                n1->y += ny * suprapunere * forta;
                n2->x -= nx * suprapunere * forta;
                n2->y -= ny * suprapunere * forta;
                if (n1->y > LIMITA_JOS_ARBORE) n1->y = LIMITA_JOS_ARBORE;
                if (n2->y > LIMITA_JOS_ARBORE) n2->y = LIMITA_JOS_ARBORE;
            }
        }
    }
}

int main() {
    initwindow(1200, 760, (char*)"Evaluator", 0, 0, true);
    bool ruleazaAplicatia = true;

    while (ruleazaAplicatia) {
        setbkcolor(LIGHTGRAY);
        cleardevice();

        setcolor(BLACK);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 4);
        settextjustify(CENTER_TEXT, TOP_TEXT);
        outtextxy(600, 50, (char*)"- Evaluator Expresii Matematice -");

        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
        settextjustify(LEFT_TEXT, TOP_TEXT);
        char labelInput[] = "Introduceti expresia: ";
        outtextxy(100, 200, labelInput);

        swapbuffers();

        string expresie = citireGrafica(100 + textwidth(labelInput), 200, 700);
        string mesajEroareSintaxa;

        if (!VerificaExpresieAvansat(expresie, mesajEroareSintaxa)) {
            bool eroareRezolvata = false;
            while (!eroareRezolvata) {
                setbkcolor(LIGHTGRAY);
                cleardevice();
                setcolor(RED);
                settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
                settextjustify(CENTER_TEXT, TOP_TEXT);
                outtextxy(600, 200, (char*)"Eroare Sintaxa");
                settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
                setcolor(BLACK);

                int yEroare = 260;
                string copieMesaj = mesajEroareSintaxa;
                string separator = " | ";
                size_t pos = 0;
                while ((pos = copieMesaj.find(separator)) != string::npos) {
                    string linie = copieMesaj.substr(0, pos);
                    outtextxy(600, yEroare, (char*)linie.c_str());
                    yEroare += 25;
                    copieMesaj.erase(0, pos + separator.length());
                }
                outtextxy(600, yEroare, (char*)copieMesaj.c_str());
                yEroare += 40;

                int btnW = 200, btnH = 50;
                bool butonApasat = false;
                int btnX = 500, btnY = (yEroare <= 710) ? yEroare : 700;

                bool hover = mousex() >= btnX && mousex() <= btnX + btnW && mousey() >= btnY && mousey() <= btnY + btnH;
                DeseneazaButon(btnX, btnY, btnW, btnH, (char*)"REWRITE", hover ? LIGHTRED : RED, WHITE);

                if (ismouseclick(WM_LBUTTONDOWN)) {
                    int mx, my;
                    getmouseclick(WM_LBUTTONDOWN, mx, my);
                    if (mx >= btnX && mx <= btnX + btnW && my >= btnY && my <= btnY + btnH) butonApasat = true;
                }
                if (butonApasat) eroareRezolvata = true;
                swapbuffers();
                delay(30);
            }
            continue;
        }

        vector<string> ExpresieFinal, ExpresieShunting;
        Despartire(expresie, ExpresieFinal);
        Algoritm_Shunting_Yard(ExpresieFinal, ExpresieShunting);

        map<string, double> variabile;
        variabile["pi"] = 3.14159265;
        variabile["e"] = 2.71828182;
        variabile["inf"] = INFINITY;
        variabile["infinit"] = INFINITY;
        variabile["infinity"] = INFINITY;
        vector<string> istoricVariabile;
        int varY = 250;

        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
        settextjustify(LEFT_TEXT, TOP_TEXT);
        for (const string& t : ExpresieFinal) {
            if (isalpha(t[0]) && !Functie(t) && variabile.find(t) == variabile.end()) {
                char buffer[100];
                sprintf(buffer, "%s = ", t.c_str());

                setbkcolor(LIGHTGRAY);
                cleardevice();
                setcolor(BLACK);
                settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 4);
                settextjustify(CENTER_TEXT, TOP_TEXT);
                outtextxy(600, 50, (char*)"- Evaluator Expresii Matematice -");
                settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
                settextjustify(LEFT_TEXT, TOP_TEXT);
                outtextxy(100, 200, labelInput);
                outtextxy(100 + textwidth(labelInput), 200, (char*)expresie.c_str());
                settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
                int yIstoric = 250;
                for (const string& linie : istoricVariabile) {
                    outtextxy(100, yIstoric, (char*)linie.c_str());
                    yIstoric += 30;
                }
                outtextxy(100, varY, buffer);
                swapbuffers();

                string val = citireGrafica(100 + textwidth(buffer), varY, 200);
                double valoareNumerica = val.empty() ? 0 : stod(val);
                variabile[t] = valoareNumerica;
                istoricVariabile.push_back(string(buffer) + val);
                varY += 30;
            }
        }

        NodArbore* rad = ConstruireArbore(ExpresieShunting);
        if (rad) {
            SimplificaArboreVizual(rad);
            int h = getInaltime(rad);
            int startY = 80;
            int pasY = (h > 0) ? (LIMITA_JOS_ARBORE - startY) / (h + 1) : 60;
            if (pasY > 70) pasY = 70;
            if (pasY < 30) pasY = 30;
            SeteazaPozitiiInitiale(rad, 600, startY, 1200 / (pow(2, min(h, 4)) + 1) * 2, pasY);
            for (int k = 0; k < 100; k++) RezolvaSuprapuneri(rad);
        }
        double rezultat = CalculeazaExpresia(ExpresieShunting, variabile);

        bool inapoiLaStart = false;
        int btnW = 120, btnH = 40;
        int resetX = 750, resetY = 680;
        int retryX = 900, retryY = 680;
        int backX = 1050, backY = 680;
        NodArbore* nodMutat = nullptr;
        bool seMuta = false;
        clearmouseclick(WM_LBUTTONDOWN);

        while (!inapoiLaStart) {
            if (kbhit() && getch() == 27) {
                ruleazaAplicatia = false;
                break;
            }

            setbkcolor(LIGHTGRAY);
            cleardevice();

            if (seMuta) RezolvaSuprapuneri(rad);
            DeseneazaDinamic(rad);

            setcolor(DARKGRAY);
            line(0, LIMITA_JOS_ARBORE + 10, 1200, LIMITA_JOS_ARBORE + 10);
            setcolor(BLACK);
            setbkcolor(LIGHTGRAY);
            settextjustify(LEFT_TEXT, TOP_TEXT);
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
            char rezBuff[100];
            sprintf(rezBuff, "Rezultat: %.4f", rezultat);
            outtextxy(30, 660, rezBuff);

            if (!mesajEroareMatematica.empty()) {
                setcolor(RED);
                settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
                outtextxy(30, 710, (char*)mesajEroareMatematica.c_str());
            }
            else if (rad && (rad->valoare == "<" || rad->valoare == ">" || rad->valoare == "=")) {
                setcolor(BLUE);
                settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
                outtextxy(30, 710, (rezultat != 0) ? (char*)"Adevarat" : (char*)"Fals");
            }

            bool hoverReset = mousex() >= resetX && mousex() <= resetX + btnW && mousey() >= resetY && mousey() <= resetY + btnH;
            bool hoverRetry = mousex() >= retryX && mousex() <= retryX + btnW && mousey() >= retryY && mousey() <= retryY + btnH;
            bool hoverBack = mousex() >= backX && mousex() <= backX + btnW && mousey() >= backY && mousey() <= backY + btnH;

            DeseneazaButon(resetX, resetY, btnW, btnH, (char*)"RESET", hoverReset ? LIGHTBLUE : BLUE, WHITE);
            DeseneazaButon(retryX, retryY, btnW, btnH, (char*)"RETRY", hoverRetry ? LIGHTGREEN : GREEN, WHITE);
            DeseneazaButon(backX, backY, btnW, btnH, (char*)"EXIT", hoverBack ? LIGHTRED : RED, WHITE);

            if (ismouseclick(WM_LBUTTONDOWN)) {
                int mx, my;
                getmouseclick(WM_LBUTTONDOWN, mx, my);
                if (mx >= resetX && mx <= resetX + btnW && my >= resetY && my <= resetY + btnH) {
                    if (rad) {
                        int h = getInaltime(rad);
                        int startY = 80;
                        int pasY = (h > 0) ? (LIMITA_JOS_ARBORE - startY) / (h + 1) : 60;
                        if (pasY > 70) pasY = 70;
                        if (pasY < 30) pasY = 30;
                        SeteazaPozitiiInitiale(rad, 600, startY, 1200 / (pow(2, min(h, 4)) + 1) * 2, pasY);
                        for (int k = 0; k < 100; k++) RezolvaSuprapuneri(rad);
                    }
                }
                else if (mx >= retryX && mx <= retryX + btnW && my >= retryY && my <= retryY + btnH) inapoiLaStart = true;
                else if (mx >= backX && mx <= backX + btnW && my >= backY && my <= backY + btnH) {
                    inapoiLaStart = true;
                    ruleazaAplicatia = false;
                }
                else {
                    nodMutat = GasesteNod(rad, mx, my);
                    if (nodMutat) seMuta = true;
                }
            }
            if (ismouseclick(WM_LBUTTONUP)) {
                seMuta = false;
                nodMutat = nullptr;
                clearmouseclick(WM_LBUTTONUP);
            }
            if (seMuta && nodMutat) {
                nodMutat->x = mousex();
                nodMutat->y = mousey();
                if (nodMutat->y > LIMITA_JOS_ARBORE) nodMutat->y = LIMITA_JOS_ARBORE;
            }

            swapbuffers();

            delay(30);
        }
        stergereMemorie(rad);
    }
    closegraph();
    return 0;
}
