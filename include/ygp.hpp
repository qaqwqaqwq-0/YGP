#ifndef _YGP_HPP_
#define _YGP_HPP_
#include<iomanip>
#ifdef YGP_USE_BOOST
#include<boost/dynamic_bitset.hpp>
namespace ygp{typedef boost::dynamic_bitset<> bitset;}
#else
#include<bitset>
#endif
#include<fstream>
#include<cassert>
#include<csignal>
#include"ygp/base.hpp"
#include"ygp/arr.hpp"
#include"ygp/rng.hpp"
#include"ygp/strmanip.hpp"
#include"ygp/trie.hpp"
#include"ygp/win/msg.hpp"
#include"ygp/win/thread.hpp"
#include"ygp/win/io.hpp"
#ifndef YGP_DISABLE_BROOM
#include"ygp/win/broom.hpp"
#endif
#include"ygp/win/encoding.hpp"
#include"ygp/win/cnslcl.hpp"
#ifndef YGP_DISABLE_DEMENTOR
#include"ygp/win/dementor.hpp"
#endif
#include"ygp/win/shared_memory.hpp"
#include"ygp/win/crypto.hpp"
#include"ygp/win/shell.hpp"
#include"ygp/win/wndutil.hpp"
#include"ygp/win/processthreaditerator.hpp"
#ifndef YGP_DISABLE_PLAYER
#include"ygp/win/player.hpp"
#endif
#ifndef YGP_DISABLE_WEBBROWSER
#include"ygp/win/webbrowser.hpp"
#endif
#ifndef YGP_DISABLE_GUI
#include"ygp/win/gui/debugconsole.hpp"
#include"ygp/win/gui/msgbox.hpp"
#include"ygp/win/gui/window.hpp"
#include"ygp/win/gui/dialog.hpp"
#endif
#endif