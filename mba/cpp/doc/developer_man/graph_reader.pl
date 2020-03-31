#!/bin/perl -w

# $Id: graph_reader.pl,v 1.1 2001/07/27 08:10:57 bhudson Exp $
# Reads the description of configure.in and generates a file appropriate
# for AT&T's dot program (look for graphviz).

open(DOT, ">config.dot") or die "Can't open config.dot: $!";
print DOT "# Automatically generated from @ARGV\n";
print DOT "# by $0 on ", scalar(gmtime()), "\n\n";

sub print_dot($) {
    my $string = shift @_;
    $string =~ s/^/    /;
    print DOT $string, "\n";
}

sub warning($) {
    print STDERR "warning at line $.: ", shift @_, "\n";
}

sub error($) {
    print STDERR "error at line $.: ", shift @_, "\n";
    exit 1;
}


###########################################################################
# Hash of declared variables.
# Hashes to the type name; set-by-user or internal-node.
my(%declared_names);

sub is_declared($) {
    my $node = shift @_;
    if(defined($declared_names{$node})) {
        return $declared_names{$node};
    } else {
        return "";
    }
}

###########################################################################
# Creating nodes.
# We need to declare them ahead of time, so we know what colour to draw
# edges and nodes.

sub create_node($$$) {
    my $attr = shift @_;
    my $node = shift @_;
    my $type = shift @_;
    if(is_declared($node)) {
        warning("$node already declared " . is_declared($node));
    }
    else {
        $declared_names{$node} = $type;
        print_dot("$node [ "
            ."label=\"$node\" "
            ."fontname=\"Arial\" "
            .$attr
            ."];");
    }
}

sub cmd_set_by_user($) {
    my $node = shift @_;
    create_node('color="green" style="filled"', $node, "set-by-user");
}
sub cmd_internal_node($) {
    my $node = shift @_;
    create_node('', $node, "internal-node");
}



###########################################################################
# Creating edges.

sub create_edge($@) {
    my $attr = shift @_;

    # check that the nodes exist
    foreach (@_) {
        if(!is_declared($_)) {
            warning("node $_ not declared");
            return;
        }
    }

    my $from      = shift @_;
    my @to_nodes  = @_;
    foreach my $to (@to_nodes) {
        print_dot("$from -> $to [$attr];");
    }
}

sub cmd_requires(@) {
    create_edge('color="yellow"', @_);
}
sub cmd_wants(@) {
    create_edge('color="green"', @_);
}
sub cmd_disallows(@) {
    create_edge('color="red"', @_);
}


###########################################################################
# Parsing the file.

my %functions = (
    "requires"          => 1,
    "wants"             => 1,
    "disallows"         => 1
);

print DOT "digraph mygraph {\n";
my $node_type = "none";
my $brace_level  = 0;
while(<>) {
    chomp;
    s/\s+/ /;
    s/#.*//;
    tr/-/_/;
    tr/^\s+//;
    next if !length;
    if(/internal_node {/) {
        $node_type = "internal-node";
    }
    elsif(/set_by_user {/) {
        $node_type = "set-by-user";
    }
    elsif(/}/) {
        error("unexpected close-brace") if $node_type eq "none";
        $node_type = "none";
    }
    else {
        my @line = split;
        if($node_type eq "set-by-user") {
            foreach my $token (@line) { cmd_set_by_user($token); }
        } elsif($node_type eq "internal-node") {
            foreach my $token (@line) { cmd_internal_node($token); }
        } else {
            my $from_node = shift @line;
            my $command   = shift @line;
            my(@to_nodes);
            push(@to_nodes, @line);
            if(!defined($functions{$command})) {
                error("unrecognized command `$command'");
            }
            else {
                my $str = "&cmd_".$command."(";
                $str .= "'$from_node'";
                foreach (@to_nodes) {
                    $str .= ", '$_'";
                }
                $str .= ")";
                eval "$str";
            }
        }
    }
}

print DOT "}\n";
close(DOT);
