using Editor.Control.TreeView;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace Editor.Control.Text
{
    public class TextEditable:System.Windows.Controls.Control
    {
         static TextEditable() 
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(TextEditable), new FrameworkPropertyMetadata(typeof(TextEditable)));
        }
        public static DependencyProperty TextProperty = DependencyProperty.Register
            ("Text",
            typeof(string),
            typeof(TextEditable),
            new FrameworkPropertyMetadata("")
            );
        public static DependencyProperty IsEditableProperty = DependencyProperty.Register
            (
            "IsEditable",
            typeof(bool),
            typeof(TextEditable),
            new FrameworkPropertyMetadata(OnIsEditableChanged) { DefaultUpdateSourceTrigger= System.Windows.Data.UpdateSourceTrigger.PropertyChanged}
            );
        public static DependencyProperty TextChangedCommandProperty = DependencyProperty.Register
            (
            "TextChangedCommand",
            typeof(ICommand),
            typeof(TextEditable),
            new FrameworkPropertyMetadata(null)
            );
        public static DependencyProperty CommandParameterProperty = DependencyProperty.Register
            (
            "CommandParameter",
            typeof(object),
            typeof(TextEditable),
            new FrameworkPropertyMetadata(null)
            );
        public object CommandParameter
        {
            get { return (object)(GetValue(CommandParameterProperty)); }
            set { SetValue(CommandParameterProperty, value); }
        }
         public ICommand TextChangedCommand
        {
            get { return (ICommand)GetValue(TextChangedCommandProperty); }
            set { SetValue(TextChangedCommandProperty, value); }
        }
        public string Text
        {
            get { return (string)GetValue(TextProperty); }
            set { SetValue(TextProperty, value); }
        }
        public bool IsEditable
        {
            get { return (bool)GetValue(IsEditableProperty); }
            set { SetValue(IsEditableProperty, value); }
        }
       
        public TextEditable()
        {
            this.LostFocus += TextEditable_LostFocus;
            //this.TextInput
        }

        

        private void TextEditable_LostFocus(object sender, RoutedEventArgs e)
        {
            if (IsEditable)
            {
                IsEditable = false;
            }
        }
        string oldText;

        System.Windows.Controls.TextBox _textBox;
        public override void OnApplyTemplate()
        {
            _textBox = (System.Windows.Controls.TextBox)GetTemplateChild("PART_TextBox");
            _textBox.IsVisibleChanged += _textBox_IsVisibleChanged;
            base.OnApplyTemplate();
        }

       

        private void _textBox_IsVisibleChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if ((bool)e.NewValue == true)
            {
                this._textBox.Focus();
                this._textBox.Select(0, this.Text.Length);
            }
        }

        private static  void  OnIsEditableChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            bool isEditable = (bool)e.NewValue;
            TextEditable textEditable = (TextEditable)d;
            if (!isEditable)
            {
                if (textEditable.oldText != textEditable.Text&&!string.IsNullOrWhiteSpace(textEditable.Text))
                {
                    TextChangedEventArgs renameEventArgs = new TextChangedEventArgs()
                    {
                        NewName = textEditable.Text
                        ,CommandParameter=textEditable.CommandParameter,
                        OldName = textEditable.oldText
                    };
                    textEditable.TextChangedCommand?.Execute(renameEventArgs);
                    //textEditable.IsEditable = false;
                }
            }
            else
            {
                textEditable.oldText= textEditable.Text;
            }
        }
    }
}
