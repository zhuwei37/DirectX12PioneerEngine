using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Application = System.Windows.Application;

namespace Editor.Server
{
    public static class ThemeManager
    {
        private static Dictionary<string, ResourceDictionary> _themes = new();

        public static void RegisterTheme(string themeName, string assemblyName, string resourcePath)
        {
            string uri = $"/{assemblyName};component/{resourcePath}";

            ResourceDictionary resource = new ResourceDictionary();
            resource.Source = new Uri(uri, UriKind.RelativeOrAbsolute);

            _themes.Add(themeName, resource);
        }

        public static void ApplyTheme(string themeName)
        {
            ResourceDictionary resource = _themes[themeName];

            Application.Current.Resources.MergedDictionaries.Add(resource);
        }
    }
}
