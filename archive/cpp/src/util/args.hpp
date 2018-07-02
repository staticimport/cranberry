#pragma once

#include "common.hpp"

#include <list>
#include <experimental/optional>

namespace CBC {

/* functionality */
class InvalidArgsException : public std::runtime_error
{
public:
    InvalidArgsException(std::string const& what);
};

class Args
{
public:
  Args(void);
  ~Args(void) {};

  // specification
  void AddMandatoryParam(char const* paramName, char const* desc);
  void AddOptionalFlag(char const* flagShort, char const* flagLong, char const* desc);
  void AddOptionalParam(char const* flagShort, char const* flagLong, char const* paramName,
                        char const* desc);
  void PrintUsage(void) const;
  void SetVersionInfo(char const*);
  // TODO: add arbitrary lambdas for validation, e.g. help or version should be lone flags

  // actual args
  void Parse(int argc, char const** argv);
  std::string GetMandatoryParam(std::string const& name) const;
  bool        GetOptionalParam(std::string const& name, std::string& out) const;
  bool        IsOptionalFlagSet(std::string const& name) const;
private:
  struct Mandatory
  {
    std::string const mParamName;
    std::string const mDescription;
    std::experimental::optional<std::string> mValue;
    Mandatory(char const* paramName, char const* desc);
  };

  class Optional
  {
  public:
    std::string const mShortName;
    std::string const mLongName;
    std::string const mDescription;

    Optional(char const* shortName, char const* longName, char const* desc);
    Optional(char const* shortName, char const* longName, char const* paramName, char const* desc);

    // flag or param?
    std::string const& GetParamName(void) const;  // will throw if flag-only
    std::string const& GetValue(void) const;      // will throw if flag-only
    bool IsFlagOnly(void) const;
    bool IsSet(void) const;
    void SetFlag(void);                           // will throw if not flag-only or already set
    void SetParam(std::string const&);            // will throw if flag-only or already set
  private:
    std::string const mParamName;
    std::experimental::optional<std::string> mValue;
  };

  void ProcessOptionalByLongName(std::string const&);
  void ProcessOptionalByShortName(std::string const&, std::list<std::string>&);
  bool TakeOptionalArg(std::list<std::string>&);

  void PrintVersion(void) const;

  std::list<Mandatory> mMandatories;
  std::list<Optional>  mOptionals;
  std::string          mVersionInfo;
};


/* implementation */
InvalidArgsException::InvalidArgsException(std::string const& what)
  : std::runtime_error(what.c_str())
{}

Args::Args(void)
{
  AddOptionalFlag("h", "help",    "print usage information");
  AddOptionalFlag("v", "version", "print version information");
}

void Args::AddMandatoryParam(char const* paramName, char const* desc)
{
  mMandatories.emplace_back(paramName, desc);
}

void Args::AddOptionalFlag(char const* flagShort, char const* flagLong, char const* desc)
{
  mOptionals.emplace_back(flagShort, flagLong, desc);
}

void Args::AddOptionalParam(char const* flagShort, char const* flagLong,
                            char const* paramName, char const* desc)
{
  mOptionals.emplace_back(flagShort, flagLong, paramName, desc);
}

void Args::SetVersionInfo(char const* version)
{
  mVersionInfo = version;
}

void Args::PrintUsage(void) const
{
  // TODO: this is functional but pretty crappy

  // summary
  std::cout << "ARGS: [optional flags/params...]";
  for (auto const& m : mMandatories) { std::cout << " <" << m.mParamName << '>'; }
  std::cout << "\n\n";

  // optionals
  std::cout << "optional:" << std::endl;
  for (auto const& o : mOptionals)
  {
    if (o.IsFlagOnly()) { std::cout << "\t-" << o.mShortName << " or --" << o.mLongName << std::endl; }
    else
    {
      std::cout << "\t-" << o.mShortName << " <" << o.GetParamName() << "> or --"
                << o.mLongName << "=<" << o.GetParamName() << '>' << std::endl;
    }
    std::cout << "\t\t" << o.mDescription << std::endl;
  }
  std::cout << "\n\n";

  // mandatories
  std::cout << "mandatory:" << std::endl;
  for (auto const& m : mMandatories)
  {
    std::cout << "\t<" << m.mParamName << '>' << std::endl
              << "\t\t" << m.mDescription << std::endl;
  }
}

void Args::PrintVersion(void) const
{
  std::cout << mVersionInfo << std::endl;
}

void Args::Parse(int argc, char const** argv)
{
  // push args onto list
  std::list<std::string> args;
  for (int idx = 1; idx != argc; ++idx) { args.push_back(argv[idx]); }

  // optional flags/params
  while (TakeOptionalArg(args)) { continue; }

  // help or version?
  if (IsOptionalFlagSet("help"))    { PrintUsage();   exit(0); }
  if (IsOptionalFlagSet("version")) { PrintVersion(); exit(0); }

  // mandatory
  if (args.size() != mMandatories.size())
  {
    throw InvalidArgsException(FormatStr(
          "Incorrect number of parameters provided: expected %zu but found %zu", mMandatories.size(), args.size()));
  }
  for (auto& m : mMandatories) { m.mValue = args.front(); args.pop_front(); }
}

std::string Args::GetMandatoryParam(std::string const& name) const
{
  for (auto const& m : mMandatories) { if (m.mParamName == name) { return *m.mValue; } }
  throw InvalidArgsException(FormatStr("Unknown arg '%s'", name.c_str()));
}

bool Args::GetOptionalParam(std::string const& name, std::string& out) const
{
  for (auto const& o : mOptionals)
  {
    if (o.mLongName != name) { continue; }
    if (!o.IsSet()) { return false; }
    out = o.GetValue();
    return true;
  }
  throw InvalidArgsException(FormatStr("Unknown arg '%s'", name.c_str()));
}

bool Args::IsOptionalFlagSet(std::string const& name) const
{
  for (auto const& o : mOptionals)
  {
    if (o.mLongName == name) { return o.IsSet(); }
  }
  throw InvalidArgsException(FormatStr("Unknown arg '%s'", name.c_str()));
}

bool Args::TakeOptionalArg(std::list<std::string>& args)
{
  // is there an optional?
  if (args.empty()) { return false; }
  std::string const arg = args.front(); // copy
  if (arg.size() < 2) { throw InvalidArgsException(FormatStr("Invalid argument '%s'", arg.c_str())); }
  if (arg.at(0) != '-') { return false; } // must be mandatory param
  args.pop_front();

  // process by short vs long
  if (arg.at(1) == '-') { ProcessOptionalByLongName(arg.substr(2)); }
  else                  { ProcessOptionalByShortName(arg.substr(1), args); }
  return true;
}

void Args::ProcessOptionalByShortName(std::string const& arg, std::list<std::string>& args)
{
  // find
  Optional* o = nullptr;
  for (auto& x : mOptionals) { if (x.mShortName == arg) { o = &x; break; } }
  if (!o) { throw InvalidArgsException(FormatStr("Invalid argument '%s'", arg.c_str())); }

  // flag only?
  if (o->IsFlagOnly()) { o->SetFlag(); return; }

  // param
  if (args.empty()) { throw InvalidArgsException(FormatStr("Flag '%s' is missing parameter", arg.c_str())); }
  o->SetParam(args.front());
  args.pop_front();
}

void Args::ProcessOptionalByLongName(std::string const& arg)
{
  size_t const eqIndex = arg.find('=');
  std::string const argName = arg.substr(0, eqIndex);

  // find
  Optional* o = nullptr;
  for (auto& x : mOptionals) { if (x.mLongName == argName) { o = &x; break; } }
  if (!o) { throw InvalidArgsException(FormatStr("Invalid argument '%s'", arg.c_str())); }

  // flag only?
  if (o->IsFlagOnly()) { o->SetFlag(); return; }

  // param
  if (eqIndex == std::string::npos) { throw InvalidArgsException(FormatStr("Flag '%s' is missing parameter", arg.c_str())); }
  o->SetParam(argName.substr(eqIndex+1, argName.length()));
}

Args::Mandatory::Mandatory(char const* paramName, char const* desc)
  : mParamName(paramName)
  , mDescription(desc)
{}

Args::Optional::Optional(char const* shortName, char const* longName, char const* desc)
    : mShortName(shortName)
    , mLongName(longName)
    , mDescription(desc)
{
  ASSERT(IsFlagOnly());
}

Args::Optional::Optional(char const* shortName, char const* longName, char const* paramName, char const* desc)
    : mShortName(shortName)
    , mLongName(longName)
    , mDescription(desc)
    , mParamName(paramName)
{
  ASSERT(!IsFlagOnly());
}

std::string const& Args::Optional::GetParamName(void) const
{
  ASSERT(!IsFlagOnly());
  return mParamName;
}

std::string const& Args::Optional::GetValue(void) const
{
  ASSERT(!IsFlagOnly());
  return *mValue;
}

PURE_FCN
bool Args::Optional::IsFlagOnly(void) const
{
  return mParamName.empty();
}

PURE_FCN
bool Args::Optional::IsSet(void) const
{
  return static_cast<bool>(mValue);
}

void Args::Optional::SetFlag(void)
{
  ASSERT(IsFlagOnly());
  if (mValue) { throw InvalidArgsException(FormatStr("Arg '%s' set repeatedly", mLongName.c_str())); }
  mValue = "X"; // doesn't matter
}

void Args::Optional::SetParam(std::string const& x)
{
  ASSERT(!IsFlagOnly());
  if (mValue) { throw InvalidArgsException(FormatStr("Arg '%s' set repeatedly", mLongName.c_str())); }
  mValue = x;
}

}
