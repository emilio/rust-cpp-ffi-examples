// The my_ffi::Box and my_ffi::OwnedSlice implementations rely on sharing the
// allocator with the rust code. Otherwise we'd need to call into Rust to free
// our stuff.
#[global_allocator]
static A: std::alloc::System = std::alloc::System;

mod owned_slice;

use owned_slice::OwnedSlice;

#[derive(Clone, Debug)]
#[repr(C, u8)]
pub enum TreeNode {
    /// This node just has a value.
    Leaf(f32),
    /// This node sums all the children.
    Sum(OwnedSlice<TreeNode>),
    /// This node returns 1 if the two things are the same, and zero otherwise.
    Cmp(Box<TreeNode>, Box<TreeNode>),
}

extern "C" {
    fn DoTheMathInCPlusPlus(node: *const TreeNode) -> f32;
}

fn main() {
    let my_complex_node = TreeNode::Sum(vec![
        TreeNode::Leaf(1.0),
        TreeNode::Cmp(
            Box::new(TreeNode::Leaf(0.0)),
            Box::new(TreeNode::Sum(vec![
                TreeNode::Leaf(2.0),
                TreeNode::Leaf(-1.0),
                TreeNode::Leaf(-5.0),
                TreeNode::Leaf(4.0),
            ].into()))
        )
    ].into());

    assert_eq!(unsafe { DoTheMathInCPlusPlus(&my_complex_node) }, 2.0);
}
