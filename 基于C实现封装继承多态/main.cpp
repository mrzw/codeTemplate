#include <cstdio>
#include <cstdlib>

typedef void (* name)();

typedef struct animal{
    int Ani;
    name name_;
};


void animal_name()
{
    printf("%s","i am a animal");
}

animal& animal_construct(animal* pthis)
{
    pthis->name_ = animal_name;
    return *pthis;
}


typedef struct dog{
    animal ani_;
    int Dog;
    name name_;
};

void dog_name()
{
    printf("%s","i am a dog");
}

dog& dog_construct(dog* pthis)
{
    pthis->name_ = dog_name;
    return *pthis;
}



int main() {
    animal* a = (animal*)malloc(sizeof(animal));
    animal_construct(a);
    dog* b = (dog*)malloc(sizeof(dog));
    animal_construct(&b->ani_);
    dog_construct(b);
    a->name_();
    printf("\n");
    b->name_();
    return 0;
}
