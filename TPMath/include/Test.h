#ifndef TEST_H
#define TEST_H


class Test
{
    public:
        Test();
        virtual ~Test();
        int Getplop() { return plop; }
        void Setplop(int val) { plop = val; }
    protected:
    private:
        int plop;
};

#endif // TEST_H
