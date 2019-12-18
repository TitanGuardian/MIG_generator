
#include <iostream>
#include <fstream>
#include <sstream>

#include "mig.h"


int main () {
    // создание базы данных, и прогрузка уже существующей
    BestSchemasDict ml("data");



    // клиент поиска мутаций (NPN)
    SearchMutation sm;


    // добавить в библиотеку текстовую базу MIG при этом база будет содержать только лучшие реализации минкодов
    // если реализован не минкод, то автоматически добавится схема соответсвующего минкода
    // можно добавить 4 ый аргумент функции false, наличие которого будет сигналом для вычисления функции, которую
    // реализует реализует схема, взятая из текстового файла
    MIG::bunch_check_update("file_with_migs", ml,sm);

    // пример поиска по БД схем сложности 17
    // тип ml.dict --- std::map<uint32_t, MIG>, где ключ это минкод
    for (auto & el : ml.dict) {
        if (el.second.complexity == 17 ) {
            // поля у структуры можно посмотреть в файле mig.h
            std::cout<<el.second.vector<<"\n";
            // для вывода MIG есть метод to_string()
            std::cout<<el.second.to_string();
        }
    }
    // пример вывода статистики по сложностям; выводится в файл "название БД"_stat.txt
    ml.out_statistic("Message");
    // можно выгрузить БД в текстовый файл
    ml.save_to_txt("file_to_save.txt");


    // можно получить MIG любой функции (!!!если есть полная БД)
    auto mig = sm.get_MIG(123456789,ml);
    // узнать ее сложность
    mig.complexity;
    // или вывести в консоль
    std::cout << mig.to_string();

    // в конце программы обновленная БД сохранится
    return 0;
}



//

//    std::cout << "hfak";

//    std::cout <<sm.find_mincode(12).vector;

//    PermutationGenerator32::generate_permutation1();


//    my_lib.out_statistic("add aig db");


//    Simple_Generator my_gen(4);

//    MIG m;
//    m.compute();
//    std::cout << m.is_correct();

//    Permutation<6> a({1,4,5,2,0,3}), b({3,0,2,5,4,1}),c,d;
//
//
//    a.print();
//    b.print();
//    (a*b).print();
//    (c = ((a*b)*(Permutation<6>({5,4,3,2,1,0})))).print();
//    (d = c.reverse()).print();
//    (c*d).print();
//    Permutation<15>::ident().print();
//
//    Permutation<3> v({1,0,2});
//    std::vector<bool> rec =  v.apply<bool>({true, false , false});
//    for (auto it : rec) std::cout << it;

//
//    SearchMutation my_searcher;
//    std::cout<<my_searcher.find_mincode(12300001).vector;
//    SearchMutation sm;
//    std::set<uint32_t> mincodes;
//    int prog = 0 ;
//    uint64_t crap = 4294967296 / 1000000;
//    for (uint64_t itr1 = 0 ; itr1 < 4294967296 ; ++itr1) {
//        mincodes.insert(sm.find_mincode(itr1).vector.to_ulong());
//        if (itr1%crap==0) {++prog;std::cout << prog <<std::endl; }
//        if (prog==10) break;
//    }
//    std::cout << mincodes.size();