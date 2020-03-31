{
    Variable::prop_iterator it = v.begin_props();
    for( ; it != v.end_props() ; ++it) {
        Proposition *p = *it;
        /* code in here acts on p */
    }
}
