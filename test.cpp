#include "packet.h"

using namespace std;

int main(int argc, char const *argv[])
{
    std::vector<PlayerTuple> _tmp;
    std::string _arr[] = {"0", "1", "2", "3", "4", "5", "6", "7"};
    for (int i = 0; i < 6; ++i)
    {
        _tmp.push_back(make_tuple(_arr[i], 100, false));
    }

    string packet = Packet::room(1, _tmp);

    _tmp = Packet::getRoomInfo(packet);

    for (int i = 0; i < _tmp.size(); ++i)
    {
        cout << get<0>(_tmp[i]) << " " << get<1>(_tmp[i]) << endl;
    }

    return 0;
}
