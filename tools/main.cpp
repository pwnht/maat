#include "maat/maat.hpp"
using namespace maat;
using namespace maat::loader;
using namespace maat::event;
using namespace std;
Action solve_chall(MaatEngine& engine,void* data)
{
    if(engine.info.addr != 0x404f459){
        return Action::CONTINUE;
    }
    maat::solver::SolverZ3 s;
    s.add(engine.info.branch->cond->invert());
    if(s.check()){
        auto model =s.get_model();
        engine.vars->update_from(*model);
    }
    return Action::HALT;
}
int main(){

// 针对Linux X86-32位平台创建一个符号引擎
    MaatEngine engine(Arch::Type::X64, env::OS::LINUX);
// 使用一个20个字符的符号命令行参数在偏移量0x08001000处加载代码
    vector<cst_t> vec(0x25);
    std::fill(vec.begin(), vec.end(), 'a');
    vector<Value> input=engine.vars->new_concolic_buffer("input", vec,8,1,'\n');
    std::list<std::string> libdirs({"/home/pwnht/ctf/linux_libs_64"});
    std::unordered_map<basic_string<char>,basic_string<char>> envp;
    std::unordered_map<std::string, std::string> virtual_fs={{"/home/pwnht/ctf/scrambled-up_1","/home/pwnht/ctf/scrambled-up_1"}};
    engine.load("/home/pwnht/ctf/scrambled-up_1", Format::ELF64, 0x4000000, {}, envp, virtual_fs,libdirs,{});
    engine.env->fs.get_fa_by_handle(0).write_buffer(input);

    engine.hooks.add(Event::PATH, When::BEFORE, EventCallback(solve_chall));
// 运行代码
    engine.run();
}