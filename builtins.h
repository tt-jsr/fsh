#pragma once
#include <vector>

namespace fsh
{
    class Machine;

    void RegisterBuiltIns(Machine& machine);

    /* Print[arg1, srg2, ...];
     * Print accepts a variable number of arguments
     * Returns None
     * Attributes: addnl->Boolean
     */
    ElementPtr Print(Machine&, std::vector<ElementPtr>&);
    ElementPtr Log(Machine&, std::vector<ElementPtr>&);
    IntegerPtr System(Machine&, std::vector<ElementPtr>&);
    NonePtr    LazyImport(Machine&, std::vector<ElementPtr>&);
    ElementPtr GetEnv(Machine&, std::vector<ElementPtr>&);

    /* Converts to an Integer
     * Throws exception if argument is not a String, Float or Int
     * Returns Integer
     * Example: ToInt[34.8]; ToInt["27"];
     */
    IntegerPtr ToInt(Machine&, std::vector<ElementPtr>&);

    /* Converts to an Float
     * Throws exception if argument is not a String, Float or Int
     * Returns Float
     * Example: ToFloat[34]; ToFloat["27.6"];
     */
    FloatPtr ToFloat(Machine&, std::vector<ElementPtr>&);

    /* Converts any argument to a String
     * Returns String
     * Example: ToString[34]; ToString[27.6];
     */
    StringPtr ToString(Machine&, std::vector<ElementPtr>&);

    StringPtr Type(Machine&, std::vector<ElementPtr>&);
    BooleanPtr IsString(Machine&, std::vector<ElementPtr>&);
    BooleanPtr IsInteger(Machine&, std::vector<ElementPtr>&);
    BooleanPtr IsFloat(Machine&, std::vector<ElementPtr>&);
    BooleanPtr IsList(Machine&, std::vector<ElementPtr>&);
    BooleanPtr IsMap(Machine&, std::vector<ElementPtr>&);
    BooleanPtr IsFileHandle(Machine&, std::vector<ElementPtr>&);
    BooleanPtr IsPair(Machine&, std::vector<ElementPtr>&);
    BooleanPtr IsFunction(Machine&, std::vector<ElementPtr>&);

    /*
     * Bind arguments
     * f[_1, arg2, ...];
     * When a function is call with positional arguments _1 thru _9, a
     * new function is returned.
     *
     * The arguments to bind define the arguments to the target function, (the function
     * being bound to). Position arguments _1, _2, ... define which arguments will be 
     * taken from the caller
     * Examples:
     *     f = &[arg1, arg2, arg3:
     *          Print[arg1, arg2, arg3];
     *     ];
     *
     *     foo = f["hello", _1, _2];
     *     foo Is a function that takes two arguments represented by_1, _2.
     *     f will be called as f["hello", _1, _2];
     *
     *     foo = f[_2, _1];
     *     foo Is a function that takes two arguments represented by_1, _2.
     *     f will be called as f[_2, __1];
     *     Notice the arguments have been swapped.
     *
     *     foo = f["hello", _1, "world"];
     *     foo Is a function that takes one argument.
     *     f will be called as f["hello", _1, "world"];
     *
     *   You can also bind a function attribute
     *     foo = f[_1, _2, _3, attr->value];
     *     foo Is a function that takes three arguments.
     *     f will be called as f[_1, _2, _3, attr->value];
     *
     * Returns a function
     */

    ElementPtr Copy(Machine&, std::vector<ElementPtr>&);
    ElementPtr First(Machine&, std::vector<ElementPtr>&);
    ElementPtr Second(Machine&, std::vector<ElementPtr>&);
    PairPtr CreatePair(Machine&, std::vector<ElementPtr>&);

    /********* List ****************************************/
    /* Returns an item from a list
     * Throws exception 
     * Returns Element of the list
     * Example: Part[lst, 7];
     */
    ElementPtr Item(Machine&, std::vector<ElementPtr>&);

    /* DefineRecord[record_name, field1, field2, field3, ...];
     *    Creates a record
     *    Throws exception
     *    Returns None
     *    Example: DefineRecord["fstat", type, mode, size, ctime, name];
     */
    NonePtr DefineRecord(Machine&, std::vector<ElementPtr>&);

    /* MakeRecord*/
    /* Make a record defined with Define record
     * Example:CreateRecord["fstat", field1, field2, ...];
     */
    ListPtr CreateRecord(Machine&, std::vector<ElementPtr>&);

    ListPtr CreateList(Machine&, std::vector<ElementPtr>&);
    
    /* Returns number of elements in a String, List or Map */
    IntegerPtr Len(Machine&, std::vector<ElementPtr>&);

    // Append an item to a list or string. Returns the index of the new item
    // Append[list, items,...]
    // Append[string, string]
    IntegerPtr Append(Machine&, std::vector<ElementPtr>&);

    // Set an item
    // ListPtr Set[list, index, item];
    // // Modifies and returns the existing list
    ListPtr Set(Machine&, std::vector<ElementPtr>&);

    ListPtr SplitList(Machine&, std::vector<ElementPtr>&);

    ListPtr Head(Machine&, std::vector<ElementPtr>&);
    ListPtr Tail(Machine&, std::vector<ElementPtr>&);

    // Push[list, item];
    ListPtr Push(Machine&, std::vector<ElementPtr>&);
    ElementPtr Pop(Machine&, std::vector<ElementPtr>&);
    ListPtr SetRecordType(Machine&, std::vector<ElementPtr>&);

    /******** Map  *********************************/
    MapPtr CreateMap(Machine&, std::vector<ElementPtr>&);

    MapPtr CreateMapFromList(Machine&, std::vector<ElementPtr>&);

    // Lookup[map, key];
    ElementPtr Lookup(Machine&, std::vector<ElementPtr>&);

    // Insert an item into a map, list, or string
    // Modifies and returns the existing map, list, or string.
    // map:
    //      Insert[map, key, value];
    // list:
    //      Insert[list, position, value1, value2...];
    // string:
    //      Insert[string, position, string];
    ElementPtr Insert(Machine&, std::vector<ElementPtr>&);

    // Delete item
    // MapPtr Delete[map, key];
    // ListPtr Delete[list, index];
    ElementPtr Delete(Machine&, std::vector<ElementPtr>&);

    /******** Error *********/
    ElementPtr IsError(Machine&, std::vector<ElementPtr>&);
    ElementPtr ErrorString(Machine&, std::vector<ElementPtr>&);
    // Throw["message"];
    ElementPtr Throw(Machine&, std::vector<ElementPtr>&);


    /********** IO ********/

    // Read a line from stdin. 
    // Input[];
    // Input[prompt];
    StringPtr Input(Machine&, std::vector<ElementPtr>&);

    // OpenFile["filename", "r"];
    FileHandlePtr OpenFile(Machine&, std::vector<ElementPtr>&);

    // OpenProcess["filename", "r"];
    // Attributes: stripnl->Boolean
    //                When reading, strring the newline
    //             addnl->Boolean
    //                When writing, add a newline
    FileHandlePtr OpenProcess(Machine&, std::vector<ElementPtr>&);
    ElementPtr PipeLine(Machine&, std::vector<ElementPtr>&);

    // ReadFile[func, "filename"];
    //      func is defined as &[line : body...];
    //      stripnl->True to strip the ending newline
    ElementPtr ReadFile(Machine&, std::vector<ElementPtr>&);

    //ReadProcess[func, "processname", "r" or "w"];
    ElementPtr ReadProcess(Machine&, std::vector<ElementPtr>&);
    ElementPtr WriteProcess(Machine&, std::vector<ElementPtr>&);

    ElementPtr WriteFile(Machine&, std::vector<ElementPtr>&);
    ElementPtr CloseFile(Machine&, std::vector<ElementPtr>&);

    /*********** String ************/
    StringPtr Trim(Machine&, std::vector<ElementPtr>&);
    StringPtr TrimLeft(Machine&, std::vector<ElementPtr>&);
    StringPtr TrimRight(Machine&, std::vector<ElementPtr>&);
    // Split["", chars];
    ListPtr Split(Machine&, std::vector<ElementPtr>&);
    StringPtr Join(Machine&, std::vector<ElementPtr>&);
    IntegerPtr Strcmp(Machine&, std::vector<ElementPtr>&);
    ElementPtr RegMatch(Machine&, std::vector<ElementPtr>&);
    ElementPtr RegSearch(Machine&, std::vector<ElementPtr>&);
    IntegerPtr Find(Machine&, std::vector<ElementPtr>&);
    StringPtr Format(Machine&, std::vector<ElementPtr>&);

    // SubString[String, pos, count]
    StringPtr SubString(Machine&, std::vector<ElementPtr>&);

    /***********************************************
     * Helpers for built in functions
     */

    ElementPtr Subscript(Machine&, std::vector<ElementPtr>&);
    ElementPtr UnitTest(Machine&, std::vector<ElementPtr>&);
    ElementPtr MachineProperty(Machine&, std::vector<ElementPtr>&);

    ElementPtr GetElement(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    StringPtr GetString(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    ListPtr GetList(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    MapPtr GetMap(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    IntegerPtr GetInteger(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    IdentifierPtr GetIdentifier(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    BooleanPtr GetBoolean(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    FunctionDefIdPtr GetFunctionDefId(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    PairPtr GetPair(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    std::string toString(Machine& machine, ElementPtr e, bool asShort = false);

    /*****************************************************************/
    struct FunctionDefinition
    {
        ElementPtr Call(Machine& machine, int64_t nArgsOnStack);
        virtual ElementPtr CallImpl(Machine&, std::vector<ElementPtr>& args) = 0;
    };
    
    struct BuiltInFunction : public FunctionDefinition
    {
        ElementPtr CallImpl(Machine&, std::vector<ElementPtr>& args);

        std::function<ElementPtr (Machine&, std::vector<ElementPtr>&)> builtIn;
    };

    struct ShellFunction : public FunctionDefinition
    {
        ElementPtr CallImpl(Machine&, std::vector<ElementPtr>& args);

        std::vector<std::string> arg_names;
        ByteCode shellFunction;
    };

    struct BoundFunction : public FunctionDefinition
    {
        ElementPtr CallImpl(Machine&, std::vector<ElementPtr>& args);

        FunctionDefinition *target;
        std::vector<ElementPtr> bound_args;
        ByteCode attributes;
    };
}
