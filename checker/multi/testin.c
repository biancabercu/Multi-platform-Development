#define VAR0 1

int main() {
    int y = VAR0 + 1;
#undef VAR0
    printf("%d\n", VAR0);

    return 0;
}